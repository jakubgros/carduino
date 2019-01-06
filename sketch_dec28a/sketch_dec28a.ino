#include "SerialJoystick.h"
#include "DistanceSensor.h"
#include "Vector.h"
#include "helpers.h"
#include "Steerage.h"
#include "pinout.h"




//TODO: calibrate
const int obstacleDistanceDetectionInCm = 5;

const int controllerMaxValForY = 60;
const int controllerMinValForY = 10;
const int controllerMaxValForX = 60;
const int controllerMinValForX = 10;
const int middlePosForY = controllerMinValForY + (controllerMaxValForY - controllerMinValForY)/2;
const int middlePosForX = controllerMinValForX + (controllerMaxValForX - controllerMinValForX)/2;






Vector controller(middlePosForX, middlePosForY); //TODO find better name
SerialJoystick joystick(&Serial);

DistanceSensor frontSensor(frontTrig, frontEcho);
DistanceSensor leftSensor(leftTrig, leftEcho);
DistanceSensor rightSensor(rightTrig, rightEcho);

Steerage car(ENA, ENB, IN1, IN2, IN3, IN4);
int sp;




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

int readSpeed() //returns val in range from 0 to 100
{
  const double scalingFactor = 100.0/(controllerMaxValForY - controllerMinValForY);
  int incline;
  
  if(controller.y <= middlePosForY) //forward
    incline =  middlePosForY-controller.y;
  else //backward
    incline = controller.y - middlePosForY;
    
  return incline * scalingFactor;
}






double readLeftTurnFactor() //output val <0;1>
{
  const double scalingFactor = 1.0/(controllerMaxValForX - middlePosForX);
  int incline;
  
  if(controller.x < middlePosForX) //controller sticked to the left
   incline =  middlePosForX-controller.x;
  else
    incline = 0;
    
  return incline; //* scalingFactor;
}

double readRightTurnFactor() //output val <0;1>
{
  const double scalingFactor = 1.0/(controllerMaxValForX - middlePosForX);
  int incline;
  
  if(controller.x > middlePosForX) //controller sticked to the left
    incline =  controller.x - middlePosForX;
  else
    incline = 0;
    
  return incline * scalingFactor;
}

direction readDirection()
{
  if(controller.y <= middlePosForY) //forward
    return direction::forward;
   else //backward
    return direction::backward;
}

void readStateOfController()
{
  joystick.readPositionIfAvailable(&controller);
  flush(&Serial);
}

bool hasFoundObstacle(int detectionDistanceInCm)
{
  return leftSensor.isCloserThan(detectionDistanceInCm) || rightSensor.isCloserThan(detectionDistanceInCm) || frontSensor.isCloserThan(detectionDistanceInCm);  
}


void loop()
{
  if(hasFoundObstacle(obstacleDistanceDetectionInCm) == false)
  {
    readStateOfController();
    
    sp = readSpeed();
    car.setSpeedOfLeftWheels(sp, readLeftTurnFactor()); 
    car.setSpeedOfRightWheels(sp, readRightTurnFactor());
    car.setDirection(readDirection()); 
  }
  else //hasFoundObstacle == true
  {
    car.clearStates(); //stop car //TODO: change it's name
    
    if(leftSensor.isCloserThan(obstacleDistanceDetectionInCm))
      car.omitObstacleOnTheLeft();
    else if(rightSensor.isCloserThan(obstacleDistanceDetectionInCm))
      car.omitObstacleOnTheRight();
    else if(frontSensor.isCloserThan(obstacleDistanceDetectionInCm)) // obstacles that are in fron of are always get round on the right side 
      car.omitObstacleInFrontOf();
  }
}
