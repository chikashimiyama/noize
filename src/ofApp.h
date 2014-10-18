#pragma once
#include <memory>
#include <map>
#include "ofMain.h"
#include "cl.hpp"
#include "const.h"
#include "ofxOpenCL.h"
#include "ofxOscReceiver.h"

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
    
protected:
        void avoidZero(float &value);

        template <typename T>
        T wrap(T target, T operand);
        void recursiveBlur(void);

        std::map<std::string, std::vector<float> > parameterMap; // osc
    
        std::vector<Parameters> parameterVector; // cl
        GlobalParameters globalParameters;
    
        std::vector<ofVec3f> renderVec, planeVec, sphereVec, tubeVec, ringVec;

        std::vector<float> sinTable;
        std::vector<float> noiseTable;
        float twistOffsetX, twistOffsetY;
    
        ofTexture recuresiveTex;
        ofFbo effect2DFbo;
        ofFbo bloom2DFbo;

        ofEasyCam camera;
        ofxOpenCL clModule;

        ofxOscReceiver oscReceiver;
        std::vector<std::string> names;

};
