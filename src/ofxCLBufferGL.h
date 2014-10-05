//
//  ofxOpenCLBufferGL.h
//
//  Created by Chikashi Miyama on 05/10/14.
//
//

#ifndef ____ofxOpenCLBufferGL__
#define ____ofxOpenCLBufferGL__

#include "ofMain.h"
#include "cl.hpp"

class ofxCLBufferGL{
public:
    ofxCLBufferGL(){};
    ofxCLBufferGL(const cl::Context &clContext, unsigned int  size, cl_mem_flags mode = CL_MEM_WRITE_ONLY);
    const ofVbo &getVbo();

    const cl::BufferGL &getCLBuffer() const;
protected:
    std::vector<ofVec3f> vertexBuffer;
    ofVbo vbo;
    cl::BufferGL clBufferGL;
};

#endif
