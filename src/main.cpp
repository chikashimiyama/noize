#include "ofMain.h"
#include "ofApp.h"
#include "const.h"
//========================================================================
int main( ){
	ofSetupOpenGL(1024, 768,OF_WINDOW);			// <-------- setup the GL context
    
    
    ofBuffer buf = ofBufferFromFile(ofToDataPath("perlin.cl"));
	ofRunApp(new ofApp(buf));

}
