#include "Vector.h"
#include "SerialJoystick.h"

SerialJoystick::SerialJoystick(Stream* stream)
{
    stream_ = stream;
}

void SerialJoystick::readPositionIfAvailable(Vector* x)
{
    if (stream_->available())
    {
        if (stream_->peek() == codePrefix_)
        {
            stream_->read();
            String str = stream_->readStringUntil(codeSuffix_);
            if (str.length() == 6)
            {
                x->x = str.substring(1, 3).toInt();
                x->y = str.substring(4, 6).toInt();
            }
        }
    }
}
