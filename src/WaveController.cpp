//
//  WaveController.cpp
//  noize
//
//  Created by Chikashi Miyama on 04/10/14.
//
//

#include "WaveController.h"


WaveController::WaveController(){
    params.resize(gNumGenerators);
}

const std::vector<Parameters> &WaveController::getParams(){
    for(int i = 0; i < gNumGenerators; i++){
        float divider = i + 1;
        params[i].ampX = ampX / divider;
        params[i].ampY = ampY / divider;
        params[i].freqX = freqX * divider;
        params[i].freqY = freqY * divider;
        params[i].offsetX += speedX * divider;
        params[i].offsetY += speedY * divider;
    }
    return params;
}

void WaveController::setAmpX(float amp){
    ampX = amp;
}

void WaveController::setAmpY(float amp){
    ampY = amp;
}

void WaveController::setFreqX(float freq){
    freqX = freq;
}

void WaveController::setFreqY(float freq){
    freqY = freq;
}

void WaveController::setSpeedX(float spd){
    speedX = spd;
}

void WaveController::setSpeedY(float spd){
    speedY = spd;
}