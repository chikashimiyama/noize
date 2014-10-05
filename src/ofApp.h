#pragma once
#include <memory>
#include "ofMain.h"
#include "cl.hpp"
#include "const.h"
#include "WaveController.h"
#include "ofxOscReceiver.h"
#include "CMOpenCL.h"

class ofApp : public ofBaseApp{
    
	public:
        ofApp(const std::string &clSource);
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
    int wrap(int target, int operand);
        float ampX;
        float ampY;
        float freqX;
        float freqY;
        std::vector<ofVec3f> initVec;
        std::vector<float> sinTable;
        std::vector<float> noiseTable;

        ofEasyCam camera;
        WaveController waveController;
        ofVbo waveVbo;
        CMOpenCL clModule;

    
        cl::BufferGL *clWaveBufferGL;
        cl::Buffer *clRandomBuffer;
        cl::Buffer *clParamBuffer;
        cl::Buffer *clSinBuffer;

    
        ofxOscReceiver oscReceiver;
};
