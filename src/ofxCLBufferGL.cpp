//
//  ofxOpenCLBufferGL.cpp
//
//  Created by Chikashi Miyama on 05/10/14.
//
//

#include "ofxCLBufferGL.h"

ofxCLBufferGL::ofxCLBufferGL(const cl::Context &clContext, unsigned int  size, cl_mem_flags mode){
    cl_int err;
    vertexBuffer.resize(size); // memory
    vbo.setVertexData(&vertexBuffer.front(), size, GL_DYNAMIC_DRAW); // OpenGL VBO
    clBufferGL = cl::BufferGL(clContext, mode, vbo.getVertId(), &err); // link vbo and openCL
    
    if(err != CL_SUCCESS){
        ofLog(OF_LOG_ERROR) << "ofxBufferGLCombo::ofxBufferGLCombo\nunable to allocate memory";
    }
}

const ofVbo &ofxCLBufferGL::getVbo(){
    return vbo;
}

const cl::BufferGL &ofxCLBufferGL::getCLBuffer() const{
    return clBufferGL;
}
