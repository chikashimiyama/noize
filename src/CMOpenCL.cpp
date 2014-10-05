//
//  CMOpenCL.cpp
//
//  Created by Chikashi Miyama on 04/10/14.
//
//

#include "CMOpenCL.h"
#include <iostream>
#include <string>

CMOpenCL::CMOpenCL(const std::string &deviceName, const std::string &clSource){
    
    // look for platform
    cl_int err = CL_SUCCESS;
    std::vector<cl::Platform> plats;
    cl::Platform::get(&plats);
    std::cout << "Number of platforms found " << plats.size();
    if(plats.size() == 0){
        std::cout << "no platform found... aborting" << std::endl;
        abort();
    }

    cl::Platform default_platform=plats[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>() << std::endl;;
    std::cout <<"create OpenCL context";
    CGLContextObj kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
    cl_context_properties properties[] = { CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup, 0};
    clContext = cl::Context(CL_DEVICE_TYPE_GPU, properties);
    std::vector<cl::Device> all_devices = clContext.getInfo<CL_CONTEXT_DEVICES>();
    if(all_devices.size() == 0){
        std::cout << "no GPU device found... aborting";
        abort();
    }

    cl::Device device;
    for(int i = 0; i < all_devices.size(); i++ ){
        if (all_devices[i].getInfo<CL_DEVICE_NAME>() ==deviceName){
        device = all_devices[i];
        break;
    }

    std::cout << "Using device: " << device.getInfo<CL_DEVICE_NAME>();
    size_t maxWorkGroupSize;
    size_t globalMemSize;
    size_t maxWorkItemDimensions;
    size_t* maxWorkItemSizes;
    device.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &maxWorkGroupSize);
    device.getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &globalMemSize);
    device.getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &maxWorkItemDimensions);
    maxWorkItemSizes = new size_t[maxWorkItemDimensions];
    device.getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, maxWorkItemSizes);
    std::cout << "Max work group size:" << maxWorkGroupSize;
    std::cout << "Global memory size:" << globalMemSize;
    std::cout << "Max Work Item Dimensions:" << maxWorkItemDimensions;
    std::cout << "Max Work Item Sizes:";

    for(int i = 0; i < maxWorkItemDimensions; i ++){
        std::cout << " dim" << i << ":" << maxWorkItemSizes[i];
    }
    
    devVector.push_back(device);
    
    cl::Program::Sources source(1, std::make_pair(clSource.c_str(),clSource.length()));
    clProgram = cl::Program(clContext, source);
    if(clProgram.build(devVector) != CL_SUCCESS){
        std::cout << "open cl build error." << std::endl;
        abort();
    }
    
    clCommandQueue = new cl::CommandQueue(clContext, devVector[0], 0, &err);
    clUpdateKernel = new cl::Kernel(clProgram, "update", &err);
    }
}

CMOpenCL::~CMOpenCL(){
    delete clCommandQueue;
    delete clUpdateKernel;
    delete clUpdateFunctor;
}

bool CMOpenCL::createNewBuffer(const std::string &bufferName, size_t bufferSize, cl_mem_flags flag){
    cl_int err;
    bufferMap[bufferName] = cl::Buffer(clContext, flag, bufferSize, NULL, &err);
    return err == CL_SUCCESS;
}

bool CMOpenCL::createNewGLBuffer(const std::string &bufferName, unsigned int obj,  cl_mem_flags flag){
    cl_int err;
    bufferGLMap[bufferName] = cl::BufferGL(clContext, flag, obj, &err);
    return err == CL_SUCCESS;
}

size_t CMOpenCL::getNumberOfBuffer(){
    return bufferMap.size();
}

size_t CMOpenCL::getNumberOfGLBuffer(){
    return bufferGLMap.size();
}







