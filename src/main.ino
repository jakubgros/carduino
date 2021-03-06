#include "SerialJoystick.h"
#include "DistanceSensor.h"
#include "Vector.h"
#include "helpers.h"
#include "Steerage.h"
#include "pinout.h"

const int lengthOfRotation = 500; //TODO: calibrate
const int obstacleDistanceDetectionInCm = 20; //TODO: calibrate
const int additionalMoveLength = 250; //TODO: calibrate
const int singleStepLength = 50; //TODO: calibrate

const int omittingButton = 10;
int sp;

SerialJoystick joystick(&Serial1, 60, 10, 60, 10);
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
  Serial1.begin(9600);
  Serial1.setTimeout(10);

  //sensors
  pinMode(frontTrig, OUTPUT);
  pinMode(frontEcho, INPUT);
  pinMode(rightTrig, OUTPUT);
  pinMode(rightEcho, INPUT);
  pinMode(leftTrig, OUTPUT);
  pinMode(leftEcho, INPUT);
  pinMode(omittingButton, INPUT);
}

 bool isOmittingOn = false;
  
void makeReads()
{
  isOmittingOn = joystick.readIfOmittingOn();
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
  Serial.println(0);
  makeReads();
  if(frontSensor.isCloserThan(obstacleDistanceDetectionInCm) == false)
  {
    sp = joystick.getSpeed();
    car.setSpeedOfLeftWheels(sp, joystick.getLeftTurnFactor());
    car.setSpeedOfRightWheels(sp, joystick.getRightTurnFactor());
    car.setDirection(joystick.getDirection()); 
  }
  else if(isOmittingOn)
  {
    isOmittingOn = false;
    car.rotateRightInPlace(lengthOfRotation);
    delay(1000);

    int i = 0;
    while(leftSensor.isCloserThan(obstacleDistanceDetectionInCm + 20))
    {
        car.moveForward(singleStepLength);
        ++i;
    } 
    car.moveForward(additionalMoveLength);

    car.rotateLeftInPlace(lengthOfRotation);
    delay(1000);

    while(leftSensor.isCloserThan(obstacleDistanceDetectionInCm + 20) == false)
    {
        car.moveForward(singleStepLength);
    }
    
    while(leftSensor.isCloserThan(obstacleDistanceDetectionInCm + 20))
    {
      car.moveForward(singleStepLength);
    }
    car.moveForward(additionalMoveLength);

    car.rotateLeftInPlace(lengthOfRotation);
    delay(1000);

    while(i >= 0)
    {
        car.moveForward(singleStepLength);
        --i;
    }

    car.rotateRightInPlace(lengthOfRotation); 
   
  } 
}
