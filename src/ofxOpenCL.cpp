//
//  ofxOpenCL.cpp
//
//  Created by Chikashi Miyama on 04/10/14.
//
//

#include "ofxOpenCL.h"
#include <iostream>
#include <string>


ofxOpenCL::ofxOpenCL(const std::string &deviceName, const std::string &clSource, const std::string &kernelName){
    
    // look for platform
    cl_int err = CL_SUCCESS;
    std::vector<cl::Platform> plats;
    cl::Platform::get(&plats);
    ofLog() << "Number of platforms found " << plats.size() ;

    if(plats.size() == 0){
        ofLog() << "no platform found... aborting" ;
        abort();
    }

    cl::Platform default_platform=plats[0];
    ofLog() << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>() ;;

    
    CGLContextObj kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
    cl_context_properties properties[] = { CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup, 0};
    
    clContext = cl::Context(CL_DEVICE_TYPE_GPU, properties);
    std::vector<cl::Device> all_devices = clContext.getInfo<CL_CONTEXT_DEVICES>();
    
    if(all_devices.size() == 0){
        ofLog() << "No device found... aborting" ;
        abort();
    }else{
        ofLog() << "Number of devices found:" << all_devices.size() ;
    }

    cl::Device device;
    cl_int targetDevice;
    for(int i = 0; i < all_devices.size(); i++ ){
        ofLog() << "Device No." << i << ": " << all_devices[i].getInfo<CL_DEVICE_NAME>() ;
        if(all_devices[i].getInfo<CL_DEVICE_NAME>() == deviceName){
            device = all_devices[i];
        }
    }

    if(device.getInfo<CL_DEVICE_NAME>().empty()){
        ofLog() << "ofxOpenCL::ofxOpenCL\nNo such device found: " << deviceName ;
        abort();
    }else{
        ofLog() << "Using device: " << device.getInfo<CL_DEVICE_NAME>() ;;
        postDeviceProfile(device);
        devVector.push_back(device);
    }
    
    
    cl::Program::Sources source(1, std::make_pair(clSource.c_str(),clSource.length()));
    clProgram = cl::Program(clContext, source);
    if(clProgram.build(devVector) != CL_SUCCESS){
        ofLog() << "ofxOpenCL::ofxOpenCL\nopen cl build error." ;
        abort();
    }
    
    clCommandQueue = cl::CommandQueue(clContext, devVector[0], 0, &err);
    if(err != CL_SUCCESS){
        ofLog(OF_LOG_ERROR) << "ofxOpenCL::ofxOpenCL\nunable to create command queue.";
        abort();
    }
    
    clKernel = cl::Kernel(clProgram, kernelName.c_str(), &err);
    if(err != CL_SUCCESS){
        ofLog(OF_LOG_ERROR) << "ofxOpenCL::ofxOpenCL\nunable to create kernel.";
        abort();
    }
}

ofxOpenCL::~ofxOpenCL(){
}

void ofxOpenCL::postDeviceProfile(const cl::Device &device) const{
    std::string deviceVersion;
    std::string deviceVendor;
    std::string deviceProfile;
    size_t max_clock_frequency;
    size_t maxWorkGroupSize;
    size_t globalMemSize;
    size_t maxWorkItemDimensions;
    size_t* maxWorkItemSizes;
    
    device.getInfo(CL_DEVICE_VERSION, &deviceVersion);
    device.getInfo(CL_DEVICE_VENDOR, &deviceVendor);
    device.getInfo(CL_DEVICE_PROFILE, &deviceProfile);
    
    device.getInfo(CL_DEVICE_MAX_CLOCK_FREQUENCY, &max_clock_frequency);
    device.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &maxWorkGroupSize);
    device.getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &globalMemSize);
    device.getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &maxWorkItemDimensions);
    device.getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, maxWorkItemSizes);
    maxWorkItemSizes = new size_t[maxWorkItemDimensions];
    
    
    ofLog() << "Device Version: " << deviceVersion ;
    ofLog() << "Device Vendor: " << deviceVendor ;
    ofLog() << "Device Profile: " << deviceProfile ;
    ofLog() << "Max Frequency: " << max_clock_frequency ;
    ofLog() << "Max work group size:" << maxWorkGroupSize ;
    ofLog() << "Global memory size:" << globalMemSize ;
    ofLog() << "Max Work Item Dimensions:" << maxWorkItemDimensions ;
    ofLog() << "Max Work Item Sizes:" ;
    for(int i = 0; i < maxWorkItemDimensions; i ++){
        ofLog() << " dim" << i << ":" << maxWorkItemSizes[i] ;
    }
}

template <typename T>
void ofxOpenCL::createNewBuffer(const std::string &bufferName, const std::vector<T> &data, cl_mem_flags mode){
   // bufferMap[bufferName] = ofxCLBuffer<T>(clContext, data, mode);
}

void ofxOpenCL::createNewGLBuffer(const std::string &bufferName, unsigned int bufferSize, cl_mem_flags mode){
    bufferGLMap[bufferName] = ofxCLBufferGL(clContext, bufferSize, mode);
}

unsigned int  ofxOpenCL::getNumberOfBuffer(){
    //return bufferMap.size();
}

unsigned int  ofxOpenCL::getNumberOfGLBuffer(){
    return bufferGLMap.size();
}

void ofxOpenCL::process(){
    
    
    
    //clCommandQueue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(512,512), cl::NullRange);
    //clCommandQueue.finish();
}





