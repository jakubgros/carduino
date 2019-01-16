#include "SerialJoystick.h"
#include "DistanceSensor.h"
#include "Vector.h"
#include "helpers.h"
#include "Steerage.h"
#include "pinout.h"

const int lengthOfRotation = 320; //TODO: calibrate
const int obstacleDistanceDetectionInCm = 20; //TODO: calibrate
const int additionalMoveLength = 400; //TODO: calibrate
const int singleStepLength = 100; //TODO: calibrate

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
}




bool hasFoundObstacle(int detectionDistanceInCm)
{
  
  return leftSensor.isCloserThan(detectionDistanceInCm) || rightSensor.isCloserThan(detectionDistanceInCm) || frontSensor.isCloserThan(detectionDistanceInCm);  
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
  makeReads();
  if(frontSensor.isCloserThan(obstacleDistanceDetectionInCm) == false)
  {
    sp = joystick.getSpeed();
    car.setSpeedOfLeftWheels(sp, joystick.getLeftTurnFactor());
    car.setSpeedOfRightWheels(sp, joystick.getRightTurnFactor());
    car.setDirection(joystick.getDirection()); 
  }
  else if(isOmittingOn)//has found obstacle
  {
    isOmittingOn = false;
    //Serial.println("*left detected*");
    car.rotateRightInPlace(lengthOfRotation);
    //jedz az stracisz przeszkode
    int i = 0;
    while(leftSensor.isCloserThan(obstacleDistanceDetectionInCm + 20))
    {
        car.moveForward(singleStepLength);
        ++i;
    }
    //Serial.println("*1*");

    
    /* car.moveForward(additionalMoveLength);
    car.rotateLeftInPlace(lengthOfRotation);
    
    //jedz az zlapiesz znowu
    while(leftSensor.isCloserThan(obstacleDistanceDetectionInCm + 20) == false)
    {
        car.moveForward(singleStepLength);
    }
    //Serial.println("*2*");
    
    //jedz az stracisz znowu
    while(leftSensor.isCloserThan(obstacleDistanceDetectionInCm + 20))
    {
      car.moveForward(singleStepLength);
    }
    car.moveForward(additionalMoveLength);
    //Serial.println("*3*");
    
    //wroc na pozycje
    car.rotateLeftInPlace(lengthOfRotation);
    while(i >= 0)
    {
        car.moveForward(singleStepLength);
        --i;
    }
    //Serial.println("*4*");
    car.rotateRightInPlace(lengthOfRotation); 
    */
  } 
}
