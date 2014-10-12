#include "ofMain.h"
#include "ofApp.h"
#include "const.h"
#include "ofxOscMessageListParser.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024, 768,OF_WINDOW);			// <-------- setup the GL context
    
    std::vector<unsigned int> workItems;
    workItems.push_back(gWindowWidth);
    workItems.push_back(gWindowHeight);
    
    ofXml xml;
    if(!xml.loadFromBuffer(ofBufferFromFile(ofToDataPath("OSCMessages.xml")))){
        ofLog() << "unable to XML file. fatel error.";
        std::terminate();
    }

    std::map<std::string, std::vector<float> > map;
    if(!ofxOscMessageListParser::getSingleton().parse(xml, map)){
        ofLog() << "unable to interpret XML. fatel error.";
        std::terminate();
    }
    
    ofBuffer buf = ofBufferFromFile(ofToDataPath("perlin.cl"), &workItems);
	ofRunApp(new ofApp(buf, workItems, map));

}
