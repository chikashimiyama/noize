//
//  CMOpenCL.h
//
//  Created by Chikashi Miyama on 04/10/14.
//
//

#ifndef __noize__CMOpenCL__
#define __noize__CMOpenCL__


#include "cl.hpp"
#include <map>
// personal openCL wrapper with Mac optimizaion

class CMOpenCL{
    
public:

    // must specify device name and source code.
    CMOpenCL(const std::string &deviceName, const std::string &clSource);
    ~CMOpenCL();
    
    bool createNewBuffer(const std::string &bufferName, size_t bufferSize, cl_mem_flags flag = CL_MEM_READ_WRITE);
    bool createNewGLBuffer(const std::string &bufferName, unsigned int obj, cl_mem_flags flag = CL_MEM_READ_WRITE);
    
    size_t getNumberOfBuffer();
    size_t getNumberOfGLBuffer();

protected:
    bool updateBuffer(const std::string &bufferName, size_t size, void* dataPtr);

    cl::Program clProgram;
    cl::Context clContext;
    
    cl::CommandQueue *clCommandQueue;
    cl::Kernel *clUpdateKernel;
    cl::KernelFunctor *clUpdateFunctor;
    std::vector<cl::Device> devVector;
    std::map<std::string, cl::Buffer> bufferMap;
    std::map<std::string, cl::BufferGL> bufferGLMap;
};


#endif