#ifndef SerialJoystick_h
#define SerialJoystick_h

#include "Arduino.h"
#include "Vector.h"

class SerialJoystick
{
public:
    SerialJoystick(Stream* stream);
    void readPositionIfAvailable(Vector* x);

private:
    Stream* stream_;
    const char codePrefix_ = 'd';
    const char codeSuffix_ = '_';
};

#endif
