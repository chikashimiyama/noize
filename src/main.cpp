#include "ofMain.h"
#include "ofApp.h"
#include "const.h"
//========================================================================
int main( ){
	ofSetupOpenGL(1024, 768,OF_WINDOW);			// <-------- setup the GL context
    
    std::vector<unsigned int> workItems;
    workItems.push_back(gWindowWidth);
    workItems.push_back(gWindowHeight);
    
    ofBuffer buf = ofBufferFromFile(ofToDataPath("perlin.cl"), &workItems);
	ofRunApp(new ofApp(buf, workItems));

}
