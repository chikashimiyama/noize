#include "ofApp.h"
#include <cmath>

ofApp::ofApp(const std::string &clSource, const std::vector<unsigned int> &workItems, const std::map<std::string, std::vector<float> > &map):
clModule("GeForce GT 750M",  clSource, "update", workItems),
parameterMap(map),
twistOffsetX(0.0),
twistOffsetY(0.0){

 
    
    recuresiveTex.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    ofSetFrameRate(30);
    
    parameterVector.resize(gNumGenerators);
    
    float halfWidth = gWindowWidth / 2.0;
    float halfHeight = gWindowHeight / 2.0;

    for(int i = 0; i < gWindowWidth; i++){
        for(int j = 0; j < gWindowHeight; j++){
            renderVec.push_back(ofVec3f(i - halfWidth ,j -halfHeight, 0 ));
            planeVec.push_back(ofVec3f(i - halfWidth,j - halfHeight, 0 ));
            colorVec.push_back(ofFloatColor(1.0, 1.0, 1.0));
        }
    }
    for(int i = 0; i < gWindowWidth; i++){
        for(int j = 0; j < gWindowHeight; j++){
            float theta = (float)i / (float)(gWindowWidth) * M_PI * 2;
            float phi = (float)j / (float)(gWindowHeight) * M_PI * 2;
            float x, y, z, a;
            a = gWindowWidth / 2;
            x = cos(theta) * sin(phi) * a;
            y = sin(theta) * sin(phi) * a;
            z = cos(phi) * a;
            sphereVec.push_back(ofVec3f(x,y,z));
        }
    }
    
    for(int i = 0; i < gWindowWidth; i++){
        for(int j = 0; j < gWindowHeight; j++){
            tubeVec.push_back(ofVec3f(cos( (float)i /(float)gWindowHeight * g2PI - M_PI) * halfWidth ,
                                      sin( (float)j /(float)gWindowHeight * g2PI - M_PI) * halfHeight ,
                                      sin( (float)i /(float)gWindowHeight * g2PI - M_PI) * cos( (float)j /(float)gWindowHeight * M_PI * 2) * gWindowWidth/2 ));
        }
    }
    
    for(int i = 0; i < gWindowWidth; i++){
        for(int j = 0; j < gWindowHeight; j++){
            float theta = (float)i / (float)(gWindowWidth) * g2PI;
            float phi = (float)j / (float)(gWindowHeight) * g2PI;
            float x, y, z, a;
            x = cos(theta)  * halfWidth;
            y = sin(theta) * halfHeight;
            z = 0;
            ringVec.push_back(ofVec3f(x,y,z));
        }
    }
    
    for(int i = 0; i < gNumPixels; i++){
        noiseTable.push_back(ofRandom(-1.0, 1.0));
    }
    
    
    clModule.createNewBuffer("GlobalParameters", &globalParameters, sizeof(GlobalParameters));
    clModule.createNewBuffer("Parameters", &parameterVector.front(), sizeof(Parameters) * gNumGenerators);
    clModule.createNewBuffer("randomTable", &noiseTable.front(), sizeof(float) * gNumPixels);
    clModule.createNewBufferGL("render", renderVec, colorVec);
    clModule.createNewBufferGL("plane", planeVec);
    clModule.createNewBufferGL("sphere", sphereVec);
    clModule.createNewBufferGL("tube", tubeVec);
    clModule.createNewBufferGL("ring", ringVec);
    
    names.push_back("GlobalParameters");
    names.push_back("Parameters");
    names.push_back("randomTable");
    names.push_back("render");
    names.push_back("plane");
    names.push_back("sphere");
    names.push_back("tube");
    names.push_back("ring");

    ofSoundStreamSetup(gNumOutput, gNumInput, this, gSamplingRate, ofxPd::blockSize() * ticksPerBuffer, 3);
    puredata.setup(gNumOutput, gNumInput, gSamplingRate, ticksPerBuffer);
}


ofApp::~ofApp(){

    
}

void ofApp::setup(){
    camera.setFarClip(100000);
    
    oscReceiver.setup(50000);
    
}

template <typename T>
T ofApp::wrap(T target, T operand){
    if(operand == 0)
        return 0;
	while(target < 0)
        target += operand;
	while(target > operand)
        target -= operand;
	return target;
}

void ofApp::avoidZero(float &value){

    if(value < 0.01f &&  value > 0.0f){
        value = 0.01f;
    }
    else if(value > -0.01f &&  value < 0.0f){
        value = -0.01f;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    while(oscReceiver.hasWaitingMessages()){
        ofxOscMessage mes;
        oscReceiver.getNextMessage(&mes);
        const std::string addr = mes.getAddress();
        std::map<std::string, std::vector<float> >::iterator it = parameterMap.find(addr);
        std::vector<float> dataVector;
        if(it == parameterMap.end()){
            ofLog() << "ofApp::update" << addr << "no such parameter";
            continue;
        }else{
            for(int i = 0; i < mes.getNumArgs(); i++){
                ofxOscArgType tp = mes.getArgType(i);
                switch (tp) {
                    case OFXOSC_TYPE_FLOAT:{
                        dataVector.push_back(mes.getArgAsFloat(i));
                        break;
                    }
                    case OFXOSC_TYPE_INT32:{
                        dataVector.push_back(static_cast<float>(mes.getArgAsInt32(i)));
                        break;
                    }
                    default:{
                        ofLog(OF_LOG_ERROR) << "ofApp::update no such type";
                        break;
                    }
                }
            }
            (*it).second = dataVector;
        }
    }

    
    for(int i = 0 ; i< gNumGenerators; i++){
        float layer = static_cast<float>(i) + 1.0;
        
        parameterVector[i].freqX = parameterMap["/freqX"][i];
        parameterVector[i].freqY = parameterMap["/freqY"][i];
        
        parameterVector[i].ampX = parameterMap["/ampX"][i];
        parameterVector[i].ampY = parameterMap["/ampY"][i];
        
        avoidZero( parameterVector[i].freqX);
        avoidZero( parameterVector[i].freqY);
        
        float rotation = parameterMap["/rotate"][i];;
        float speedX = parameterMap["/speedX"][i] * cos(rotation) - parameterMap["/speedX"][i] * sin(rotation);
        float speedY = parameterMap["/speedY"][i] * sin(rotation) + parameterMap["/speedY"][i] * cos(rotation);
        
        parameterVector[i].offsetX += speedX / (gWindowWidth / parameterVector[i].freqX) * layer;
        parameterVector[i].offsetY += speedY / (gWindowHeight / parameterVector[i].freqY) * layer;
        //wrap
        parameterVector[i].offsetX = wrap(parameterVector[i].offsetX, (float)gWindowWidth);
        parameterVector[i].offsetY = wrap(parameterVector[i].offsetY, (float)gWindowHeight);
    }
    
    globalParameters.planeAmp = parameterMap["/planeAmp"][0];
    globalParameters.sphereAmp = parameterMap["/sphereAmp"][0];
    globalParameters.tubeAmp = parameterMap["/tubeAmp"][0];
    globalParameters.ringAmp = parameterMap["/ringAmp"][0];
    globalParameters.twistFreqX = parameterMap["/twistFreqX"][0];
    globalParameters.twistFreqY = parameterMap["/twistFreqY"][0];
    twistOffsetX += parameterMap["/twistSpeedX"][0];
    twistOffsetY += parameterMap["/twistSpeedY"][0];
    
    globalParameters.twistOffsetX =twistOffsetX;
    globalParameters.twistOffsetY =twistOffsetY;
    twistOffsetX = wrap(twistOffsetX, g2PI);
    twistOffsetY = wrap(twistOffsetY, g2PI);
    clModule.updateBuffer("GlobalParameters", &globalParameters, sizeof(GlobalParameters));
    clModule.updateBuffer("Parameters", &parameterVector.front(), sizeof(Parameters) * gNumGenerators);
    

    clModule.process(names);
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
    //effect2DFbo.begin(); // begin 2D recording
    recursiveBlur();
    
    camera.setPosition(parameterMap["/camera"][0], parameterMap["/camera"][1], parameterMap["/camera"][2]);
    camera.lookAt(ofVec3f(0,0,0));
    camera.begin();

    glScalef(parameterMap["/scaleX"][0] , parameterMap["/scaleY"][0], 1);
    ofEnableAlphaBlending();
    ofSetColor(ofColor::white, 255);
    clModule.getVbo("render").draw(GL_POINTS, 0, gNumPixels );
    
    camera.end();
    //effect2DFbo.end();
    recuresiveTex.loadScreenData(0, 0, ofGetWidth(), ofGetHeight());
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 5, 15);
}

void ofApp::recursiveBlur(void){
    ofPushMatrix();
    
    float blurBrightness = parameterMap["/blurBrightness"][0];
    blurBrightness;
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    glColor4f(1.0,1.0,1.0,blurBrightness);
    recuresiveTex.bind();
    glRotatef(0, 0, 0, 1);
    
    float height = ofGetHeight();
    float width = ofGetWidth();
    float blurX = parameterMap["/blurX"][0] * width / 2.0;
    float blurY = parameterMap["/blurY"][0] * height / 2.0;

    
    glBegin(GL_POLYGON);
    glTexCoord2f(0, height);
    glVertex2f(-blurX, -blurY);
    
    glTexCoord2f(width ,  height);
    glVertex2f(ofGetWidth()+blurX, -blurY);
    
    glTexCoord2f(width, 0 );
    glVertex2f(width+blurX, height+blurY);
    
    glTexCoord2f(0 , 0);
    glVertex2f(-blurX, height+blurY);
    glEnd();
    
    recuresiveTex.unbind();
    ofDisableBlendMode();
    ofPopMatrix();
    glColor4f(1.0,1.0,1.0,1.0);
}


void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
    puredata.audioReceived(input, bufferSize, gNumInput);
}

void ofApp::audioRequested(float * output, int bufferSize, int nChannels){
    puredata.audioRequested(output, bufferSize, gNumOutput);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
