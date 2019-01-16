#include "Vector.h"
#include "SerialJoystick.h"

void SerialJoystick::readPositionIfAvailable()
{
    if (stream_->available())
    {
        while(stream_->peek() != codePrefix_ && stream_->peek() != 'a')
        {
          stream_->read();
        }
        if (stream_->peek() == codePrefix_)
        {
            stream_->read();
            String str = stream_->readStringUntil(codeSuffix_);
            if (str.length())
            {
                xPos_ = str.substring(1, 3).toInt();
                yPos_ = str.substring(4, 6).toInt();
            }
        }
    }
    stream_->flush();
    //TODO sk: read while available
}

bool SerialJoystick::readIfOmittingOn()
{
    if (stream_->available())
    {
        if (stream_->peek() == 'a')
        {
            stream_->read();
            return true;
        }
        else
          return false;
    }
    stream_->flush();
    //TODO sk: read while available
}

int SerialJoystick::getSpeed() //returns val <0;100>
{
  const double scalingFactor = 100.0/(controllerMaxValForY_ - middlePosForY_);
  int incline;
  
  if(yPos_ <= middlePosForY_) //forward
    incline =  middlePosForY_ - yPos_;
  else //backward
    incline = yPos_ - middlePosForY_;
  return incline * scalingFactor;
}

double SerialJoystick::getLeftTurnFactor() //output val <0;1>
{
  const double scalingFactor = 1.0/(controllerMaxValForX_ - middlePosForX_);
  int incline;
  
  if(xPos_ < middlePosForX_) //controller sticked to the left
   incline =  middlePosForX_ - xPos_;
  else
    incline = 0;
    
  return incline * scalingFactor;
}

double SerialJoystick::getRightTurnFactor() //output val <0;1>
{
  const double scalingFactor = 1.0/(controllerMaxValForX_ - middlePosForX_);
  int incline;
  
  if(xPos_ > middlePosForX_) //controller sticked to the left
    incline =  xPos_ - middlePosForX_;
  else
    incline = 0;
    
  return incline * scalingFactor;
}

direction SerialJoystick::getDirection()
{
  if(yPos_ <= middlePosForY_) //forward
    return direction::forward;
  else //backward
    return direction::backward;
}
