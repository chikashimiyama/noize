#include "ofApp.h"
#include <cmath>

ofApp::ofApp(const std::string &clSource):
clModule("GeForce GT 750M",  clSource, "update"){
    ampX = 0;
    ampY = 0;
    freqX = 0;
    freqY = 0;
    for(int i = 0; i < gWindowWidth; i++){
        for(int j = 0; j < gWindowHeight; j++){
            initVec.push_back(ofVec3f(i,j, 0 ));
        }
    }
    
    for(int i = 0; i < gNumPixels; i++){
        noiseTable.push_back(ofRandom(-1.0, 1.0));
    }
    
    clModule.createNewBuffer("randomTable", &noiseTable.front(), sizeof(float) * gNumPixels);
    clModule.createNewBuffer("parameters", &(waveController.getParams().front()), sizeof(Parameters) * gNumGenerators);
    clModule.createNewBufferGL("vertices", initVec); // num vertices not byte
    
}


ofApp::~ofApp(){

    
}

void ofApp::setup(){
    camera.setFarClip(100000);
    camera.setPosition(0, 0, -200);
    camera.lookAt(ofVec3f(0,0,0));
    
    oscReceiver.setup(50000);
    
}

int ofApp::wrap(int target, int operand){
    if(operand == 0)
        return 0;
	while(target < 0)
        target += operand;
	while(target > operand)
        target -= operand;
	return target;
}

//--------------------------------------------------------------
void ofApp::update(){
    while(oscReceiver.hasWaitingMessages()){
        ofxOscMessage mes;
        oscReceiver.getNextMessage(&mes);
        if(mes.getAddress() == "/ampX" ){
            ampX = mes.getArgAsFloat(0);
        }else if(mes.getAddress() == "/ampY"){
            ampY = mes.getArgAsFloat(0);
        }else if(mes.getAddress() == "/freqX"){
            freqX = mes.getArgAsFloat(0);
        }else if(mes.getAddress() == "/freqY"){
            freqY = mes.getArgAsFloat(0);
        }
    }

    //ofLog() << ofGetFrameRate();
    cl_int err;
    cl::Event event;
    
    waveController.setAmpX(100);
    waveController.setAmpY(100);
    waveController.setFreqX(5 );
    waveController.setFreqY(5 );
    waveController.setSpeedX(1);
    waveController.setSpeedY(1);

    const std::vector<Parameters> &params = waveController.getParams();
    clModule.updateBuffer("parameters", &params.front(), sizeof(Parameters) * gNumGenerators);
    
    std::vector<std::string> names;
    names.push_back("parameters");
    names.push_back("randomTable");
    names.push_back("vertices");

    clModule.process(names);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    camera.begin();
    ofPushMatrix();
    glLineWidth(0.01);
    glTranslatef(-(gWindowWidth/2), 0, 0);
    ofSetColor(ofColor::white);
    clModule.getVbo("vertices").draw(GL_POINTS, 0, gNumPixels );
    ofPopMatrix();
    camera.end();
    
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 5, 15);
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
