#pragma once
#include <memory>
#include <map>
#include "ofMain.h"
#include "cl.hpp"
#include "const.h"
#include "ofxOpenCL.h"
#include "ofxOscReceiver.h"
#include "PureData.h"

class ofApp : public ofBaseApp{
    
	public:
        ofApp(const std::string &clSource, const std::vector<unsigned> &workItems, const std::map<std::string , std::vector<float> > &map);
        ~ofApp();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void audioReceived(float * input, int bufferSize, int nChannels);
        void audioRequested(float * output, int bufferSize, int nChannels);

        PureData puredata;

protected:
        void avoidZero(float &value); //  with lambda
        template <typename T>
        T wrap(T target, T operand); // with lambda
    
        void recursiveBlur(void);
        std::map<std::string, std::vector<float> > parameterMap; // osc
    
        std::vector<Parameters> parameterVector; // cl
        GlobalParameters globalParameters;
        std::vector<ofVec3f> renderVec, planeVec, sphereVec, tubeVec, ringVec;
        std::vector<ofFloatColor> colorVec;
    
        std::vector<float> sinTable;
        std::vector<float> noiseTable;
        float twistOffsetX, twistOffsetY;
    

        ofShader shader;
        ofxOpenCL clModule;
    
        ofxOscReceiver oscReceiver;
        std::vector<std::string> names;
    
        ofCamera camera;
        ofTexture recuresiveTex;
        ofFbo effect2DFbo;
        ofFbo bloom2DFbo;

};
