#include "Steerage.h"
#include "Arduino.h"

void Steerage::clearStatesOfEnginesDriversPins()
{
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, LOW);   
}

void Steerage::leftForward()
{
    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, LOW);
}

void Steerage::rightForward()
{
    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, HIGH); 
}

void Steerage::leftBackward()
{
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, HIGH); 
}

void Steerage::rightBackward()
{
    digitalWrite(IN3, HIGH); 
    digitalWrite(IN4, LOW); 
}

void Steerage::rotateLeftInPlace(int rotateLength)
{  
    clearStatesOfEnginesDriversPins();
    
    rightForward();
    leftBackward();
    analogWrite(ENA, calculateSpeed(100, 0));
    analogWrite(ENB, calculateSpeed(100, 0));
    delay(rotateLength);
    
    clearStatesOfEnginesDriversPins();
    analogWrite(ENA, calculateSpeed(0, 0));
    analogWrite(ENB, calculateSpeed(0, 0));
}

void Steerage::rotateRightInPlace(int rotateLength)
{
    clearStatesOfEnginesDriversPins();
    
    rightBackward();
    leftForward();
    analogWrite(ENA, calculateSpeed(100, 0));
    analogWrite(ENB, calculateSpeed(100, 0));
    delay(rotateLength);
    
    clearStatesOfEnginesDriversPins();
    analogWrite(ENA, calculateSpeed(0, 0));
    analogWrite(ENB, calculateSpeed(0, 0));
}

void Steerage::moveForward(int numberOfMovementUnits)
{
    clearStatesOfEnginesDriversPins();
    
    setDirection(direction::forward);
    analogWrite(ENA, calculateSpeed(50, 0));
    analogWrite(ENB, calculateSpeed(50, 0));
    
    delay(numberOfMovementUnits);

    analogWrite(ENA, calculateSpeed(0, 0));
    analogWrite(ENB, calculateSpeed(0, 0));
    clearStatesOfEnginesDriversPins();
}

void Steerage::setSpeedOfLeftWheels(int speedVal, double turnFactor, int speedUnits, int maxSpeedVal, int minSpeedVal)
{
    analogWrite(ENA, calculateSpeed(speedVal, turnFactor, speedUnits, maxSpeedVal, minSpeedVal)); 
}

void Steerage::setSpeedOfRightWheels(int speedVal, double turnFactor, int speedUnits, int maxSpeedVal, int minSpeedVal)
{
    analogWrite(ENB, calculateSpeed(speedVal, turnFactor, speedUnits, maxSpeedVal, minSpeedVal)); 
}

int Steerage::calculateSpeed(int speedVal, double turnFactor, int speedUnits, int maxSpeedVal, int minSpeedVal)
{
  int calculatedSpeed;
  if(speedVal == 0)
    calculatedSpeed = 0;
  else
    calculatedSpeed =  minSpeedVal + (1-turnFactor) * speedVal*(maxSpeedVal-minSpeedVal)/speedUnits;
    
  return calculatedSpeed;
}

void Steerage::omitObstacleOnTheLeft()
{
  clearStatesOfEnginesDriversPins();
  rotateRightInPlace(lengthOfRotation);
  moveForward(singleOmitMoveLength);
  rotateLeftInPlace(lengthOfRotation);
}

void Steerage::omitObstacleOnTheRight()
{
  clearStatesOfEnginesDriversPins();
  rotateLeftInPlace(lengthOfRotation);
  moveForward(singleOmitMoveLength);
  rotateRightInPlace(lengthOfRotation);
}

void Steerage::omitObstacleInFrontOf()
{
  clearStatesOfEnginesDriversPins();
  rotateRightInPlace(lengthOfRotation);
  moveForward(singleOmitMoveLength);
  rotateLeftInPlace(lengthOfRotation);
}

void Steerage::setDirection(direction dir)
{
  if(dir == forward) 
  {
    clearStatesOfEnginesDriversPins();
    rightForward();
    leftForward();
  }
  else //backward
  {
    clearStatesOfEnginesDriversPins();
    rightBackward();
    leftBackward();
  }
}
