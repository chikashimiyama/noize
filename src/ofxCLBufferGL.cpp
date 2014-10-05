//
//  ofxOpenCLBufferGL.cpp
//
//  Created by Chikashi Miyama on 05/10/14.
//
//

#include "ofxCLBufferGL.h"

ofxCLBufferGL::ofxCLBufferGL(const cl::Context &clContext, std::vector<ofVec3f> defaultVertices, cl_mem_flags mode){
    cl_int err;
    vertexBuffer = defaultVertices;
    vbo.setVertexData(&vertexBuffer.front(), vertexBuffer.size(), GL_DYNAMIC_DRAW); // OpenGL VBO
    clBufferGL = cl::BufferGL(clContext, mode, vbo.getVertId(), &err); // link vbo and openCL
    
    if(err != CL_SUCCESS){
        ofLog(OF_LOG_ERROR) << "ofxBufferGLCombo::ofxBufferGLCombo\nunable to allocate memory";
    }
}

ofVbo &ofxCLBufferGL::getVbo(){
    return vbo;
}

const cl::BufferGL &ofxCLBufferGL::getCLBuffer() const{
    return clBufferGL;
}
