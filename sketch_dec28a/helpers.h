#ifndef Helpers_h
#define Helpers_h

#include "Arduino.h"

inline void flush(Stream* stream)
{
    while (stream->available())
        stream->read();
}

enum direction
{
  forward,
  backward
};

#endif