#include "SerialJoystick.h"
#include "DistanceSensor.h"
#include "Vector.h"
#include "helpers.h"
#include "Steerage.h"
#include "pinout.h"

const int lengthOfRotation = 380; //TODO: calibrate
const int obstacleDistanceDetectionInCm = 10; //TODO: calibrate
const int additionalMoveLength = 100; //TODO: calibrate
const int singleStepLength = 10; //TODO: calibrate

int sp;

SerialJoystick joystick(&Serial, 60, 10, 60, 10);
DistanceSensor frontSensor(frontTrig, frontEcho);
DistanceSensor leftSensor(leftTrig, leftEcho);
DistanceSensor rightSensor(rightTrig, rightEcho);
Steerage car(ENA, ENB, IN1, IN2, IN3, IN4);

void setup() 
{
  //engine's driver
  pinMode(ENA, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(ENB, OUTPUT);

  //bluetooth
  Serial.begin(9600);
  Serial.setTimeout(10);

  //sensors
  pinMode(frontTrig, OUTPUT);
  pinMode(frontEcho, INPUT);
  pinMode(rightTrig, OUTPUT);
  pinMode(rightEcho, INPUT);
  pinMode(leftTrig, OUTPUT);
  pinMode(leftEcho, INPUT);
}




bool hasFoundObstacle(int detectionDistanceInCm)
{
  
  return leftSensor.isCloserThan(detectionDistanceInCm) || rightSensor.isCloserThan(detectionDistanceInCm) || frontSensor.isCloserThan(detectionDistanceInCm);  
}

void makeReads()
{
  joystick.readPositionIfAvailable();

  static int i = 0;
  if(i++ < 10) //only once 10 iteratios, because they take long time
  {
    frontSensor.read();
    leftSensor.read();
    rightSensor.read();
  }
  else
    i = 0;
}

void loop()
{
  makeReads();

  if(frontSensor.isCloserThan(obstacleDistanceDetectionInCm) == false)
  {
    sp = joystick.getSpeed();
    car.setSpeedOfLeftWheels(sp, joystick.getLeftTurnFactor()); 
    car.setSpeedOfRightWheels(sp, joystick.getRightTurnFactor());
    car.setDirection(joystick.getDirection()); 
  }
  else //has found obstacle
  {
    if(leftSensor.isCloserThan(obstacleDistanceDetectionInCm))
    {
      car.rotateRightInPlace(lengthOfRotation);
      //jedz az stracisz przeszkode
      int i = 0;
      while(leftSensor.isCloserThan(obstacleDistanceDetectionInCm + 5))
      {
          car.moveForward(singleStepLength);
          ++i;
      }
  
      car.moveForward(additionalMoveLength);
      car.rotateLeftInPlace(lengthOfRotation);
      //jedz az zlapiesz znowu
      while(leftSensor.isCloserThan(obstacleDistanceDetectionInCm + 5) == false)
          car.moveForward(singleStepLength);
  
      //jedz az stracisz znowu
      while(leftSensor.isCloserThan(obstacleDistanceDetectionInCm + 5))
          car.moveForward(singleStepLength);
      car.moveForward(additionalMoveLength);
  
      //wroc na pozycje
      car.rotateLeftInPlace(lengthOfRotation);
      while(i >= 0)
      {
          car.moveForward(singleStepLength);
          --i;
      }
      car.rotateRightInPlace(lengthOfRotation);
    }
  }
}
