#include "ofApp.h"
#include <cmath>

ofApp::ofApp(const std::string &clSource):
clModule("GeForce GT 750M",  clSource){
    ampX = 0;
    ampY = 0;
    freqX = 0;
    freqY = 0;
    for(int i = 0; i < gWindowWidth; i++){
        for(int j = 0; j < gWindowHeight; j++){
            initVec.push_back(ofVec3f(i,j, 0 ));
        }
    }
    
    waveVbo.setVertexData(&initVec.front(), gNumPixels, GL_DYNAMIC_DRAW );
    waveVbo.disableColors();
    waveVbo.disableNormals();
    
    for(int i = 0; i < gNumPixels; i++){
        noiseTable.push_back(ofRandom(-1.0, 1.0));
    }
    

//    
//    clCommandQueue->enqueueWriteBuffer(*clRandomBuffer, CL_TRUE, 0, sizeof(float) * gNumPixels, &noiseTable.front());
//    
//    clCommandQueue->enqueueWriteBuffer(*clSinBuffer, CL_TRUE, 0, sizeof(float) * gTableSize, &sinTable.front());
//    clUpdateFunctor = new cl::KernelFunctor(*clUpdateKernel,
//                                            *clCommandQueue,
//                                            cl::NullRange,
//                                            cl::NDRange(gWindowWidth, gWindowHeight),
//                                            cl::NullRange);

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
    
    waveController.setAmpX(ampX);
    waveController.setAmpY(ampY);
    waveController.setFreqX(freqX + 2);
    waveController.setFreqY(freqY + 2);
    waveController.setSpeedX(1);
    waveController.setSpeedY(1);

    const std::vector<Parameters> &params = waveController.getParams();
//    err = clCommandQueue->enqueueWriteBuffer(*clParamBuffer,
//                                             CL_TRUE, 0,
//                                             sizeof(Parameters)*gNumGenerators,
//                                             &params.front(),
//                                             NULL, &event);
    
    if(err != CL_SUCCESS){
        ofLog(OF_LOG_ERROR) << "ofApp::update\n cl buffer copy error";
    }
    event.wait();
    
//    (*clUpdateFunctor)( *clParamBuffer, gNumGenerators, *clRandomBuffer, *clSinBuffer, *clWaveBufferGL, &event);
    event.wait();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    camera.begin();
    ofPushMatrix();
    glLineWidth(0.01);
    glTranslatef(-(gWindowWidth/2), 0, 0);
    glRotatef(76, 1, 0 ,0);
    ofSetColor(ofColor::white);
    waveVbo.draw(GL_POINTS, 0, gNumPixels);
    
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
