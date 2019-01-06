#ifndef SerialJoystick_h
#define SerialJoystick_h

#include "Arduino.h"
#include "Vector.h"
#include "helpers.h"

class SerialJoystick
{
    const int controllerMaxValForY_;
    const int controllerMinValForY_;
    const int controllerMaxValForX_;
    const int controllerMinValForX_;

    const int middlePosForY_;
    const int middlePosForX_;

public:
    SerialJoystick(Stream* stream, const int controllerMaxValForY, const int controllerMinValForY, const int controllerMaxValForX, const int controllerMinValForX):
        stream_(stream),
        controllerMaxValForY_(controllerMaxValForY), 
        controllerMinValForY_(controllerMinValForY),
        controllerMaxValForX_(controllerMaxValForX),
        controllerMinValForX_(controllerMinValForX),
        middlePosForY_(controllerMinValForY + (controllerMaxValForY - controllerMinValForY)/2),
        middlePosForX_(controllerMinValForX + (controllerMaxValForX - controllerMinValForX)/2),
        xPos_(0),
        yPos_(0)
    {
        ;
    }

    void readPositionIfAvailable();
    
    int readSpeed(); //returns val in range from 0 to 100
    double readLeftTurnFactor(); //output val <0;1>
    double readRightTurnFactor(); //output val <0;1>
    direction readDirection();

private:
    Stream* stream_;
    const char codePrefix_ = 'd';
    const char codeSuffix_ = '_';
    int xPos_;
    int yPos_;
};

#endif
