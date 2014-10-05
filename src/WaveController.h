//
//  WaveController.h
//  noize
//
//  Created by Chikashi Miyama on 04/10/14.
//
//

#ifndef ____WaveController__
#define ____WaveController__

#include <vector>
#include "const.h"

class WaveController{
    
public:
    WaveController();
    void setAmpX(float amp);
    void setAmpY(float amp);
    void setFreqX(float freq);
    void setFreqY(float freq);
    void setSpeedX(float spd);
    void setSpeedY(float spd);
    const std::vector<Parameters> &getParams();
    
private:
    float ampX;
    float ampY;
    float freqX;
    float freqY;
    float speedX;
    float speedY;
    
    std::vector<Parameters> params;
};


#endif
