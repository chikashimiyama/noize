
#ifndef  ____const_h_
#define  ____const_h_

#include "cl.hpp"


const int gWindowWidth = 512;
const int gWindowHeight = 512;
const int gNumPixels = gWindowWidth * gWindowHeight;
const int gNumGenerators = 5;

typedef struct GlobalParameters{
    cl_float planeAmp;
    cl_float sphereAmp;
    cl_float tubeAmp;
    cl_float ringAmp;
} GlobalParameter;

typedef struct Parameters{
    cl_float freqX;
    cl_float freqY;
    cl_float ampX;
    cl_float ampY;
    cl_float offsetX;
    cl_float offsetY;
    cl_float dummy1;
    cl_float dummy2;
} Parameters;


#endif