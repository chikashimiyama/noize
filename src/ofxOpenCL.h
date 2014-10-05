//
//  ofxOpenCL.h
//
//  Created by Chikashi Miyama on 04/10/14.
//
//

#ifndef __ofxOpenCL__
#define __ofxOpenCL__

#include "ofMain.h"
#include "ofxCLBuffer.h"
#include "ofxCLBufferGL.h"
#include "cl.hpp"

class ofxOpenCL{

public:

    // must specify device name and source code.
    ofxOpenCL(const std::string &deviceName, const std::string &clSource, const std::string &kernelName);
    ~ofxOpenCL();
    
    template<typename T>
    void createNewBuffer(const std::string &bufferName, const std::vector<T> &data, cl_mem_flags mode = CL_MEM_READ_WRITE);
    void createNewGLBuffer(const std::string &bufferName, unsigned int  bufferSize, cl_mem_flags mode = CL_MEM_READ_WRITE) ;
    
    unsigned int  getNumberOfBuffer();
    unsigned int  getNumberOfGLBuffer();
    
    void process();
    
protected:
    void postDeviceProfile(const cl::Device &device) const;
    bool updateBuffer(const std::string &bufferName, unsigned int  size, void* dataPtr);

    cl::Program clProgram;
    cl::Context clContext;
    
    cl::CommandQueue clCommandQueue;
    cl::Kernel clKernel;
    std::vector<cl::Device> devVector;
    std::map<std::string, ofxCLBuffer> bufferCLMap;
    std::map<std::string, ofxCLBufferGL> bufferGLMap;

};


#endif