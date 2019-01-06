#include "Vector.h"
#include "SerialJoystick.h"

void SerialJoystick::readPositionIfAvailable()
{
    if (stream_->available())
    {
        if (stream_->peek() == codePrefix_)
        {
            stream_->read();
            String str = stream_->readStringUntil(codeSuffix_);
            if (str.length() == 6)
            {
                xPos_ = str.substring(1, 3).toInt();
                yPos_ = str.substring(4, 6).toInt();
            }
        }
    }
    
    flush(stream_);
}

int SerialJoystick::readSpeed() //returns val <0;100>
{
  readPositionIfAvailable();
  
  const double scalingFactor = 100.0/(controllerMaxValForY_ - controllerMinValForY_);
  int incline;
  
  if(yPos_ <= middlePosForY_) //forward
    incline =  middlePosForY_ - yPos_;
  else //backward
    incline = yPos_ - middlePosForY_;
    
  return incline * scalingFactor;
}

double SerialJoystick::readLeftTurnFactor() //output val <0;1>
{
  readPositionIfAvailable();
    
  const double scalingFactor = 1.0/(controllerMaxValForX_ - middlePosForX_);
  int incline;
  
  if(xPos_ < middlePosForX_) //controller sticked to the left
   incline =  middlePosForX_ - xPos_;
  else
    incline = 0;
    
  return incline * scalingFactor;
}

double SerialJoystick::readRightTurnFactor() //output val <0;1>
{
  readPositionIfAvailable();

  const double scalingFactor = 1.0/(controllerMaxValForX_ - middlePosForX_);
  int incline;
  
  if(xPos_ > middlePosForX_) //controller sticked to the left
    incline =  xPos_ - middlePosForX_;
  else
    incline = 0;
    
  return incline * scalingFactor;
}

direction SerialJoystick::readDirection()
{
  readPositionIfAvailable();
    
  if(yPos_ <= middlePosForY_) //forward
    return direction::forward;
  else //backward
    return direction::backward;
}
