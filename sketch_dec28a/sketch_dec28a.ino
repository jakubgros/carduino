#include "SerialJoystick.h"
#include "DistanceSensor.h"
#include "Vector.h"
#include "helpers.h"
#include "Steerage.h"
#include "pinout.h"

const int obstacleDistanceDetectionInCm = 10; //TODO: calibrate
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

  if(hasFoundObstacle(obstacleDistanceDetectionInCm) == false)
  {
    sp = joystick.getSpeed();
    car.setSpeedOfLeftWheels(sp, joystick.getLeftTurnFactor()); 
    car.setSpeedOfRightWheels(sp, joystick.getRightTurnFactor());
    car.setDirection(joystick.getDirection()); 
  }
  else //has found obstacle
  {
    if(leftSensor.isCloserThan(obstacleDistanceDetectionInCm))
      car.omitObstacleOnTheLeft();
    else if(rightSensor.isCloserThan(obstacleDistanceDetectionInCm))
      car.omitObstacleOnTheRight();
    else if(frontSensor.isCloserThan(obstacleDistanceDetectionInCm)) // obstacles that are in fron of are always get round on the right side 
      car.omitObstacleInFrontOf();
  }
}
