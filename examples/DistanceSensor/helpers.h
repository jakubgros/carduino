#ifndef Helpers_h
#define Helpers_h

#include "Arduino.h"

inline void flush(Stream* stream)
{
    while (stream->available())
        stream->read();
}

#endif