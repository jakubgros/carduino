#include "SerialJoystick.h"
#include "DistanceSensor.h"
#include "Vector.h"
#include "helpers.h"
#include "Steerage.h"

//ENGINE DRIVER PINOUT
const int ENA =  9; //LEWY
const int IN4 = 2;
const int IN3 = 4;
const int IN2 = 3;
const int IN1 = 5;
const int ENB = 6; //PRAWY

//DISTANCE SENSORS PINOUT
const int frontTrig = 7;
const int frontEcho = 8;
const int rightTrig = 10;
const int rightEcho = 11;
const int leftTrig = 12;
const int leftEcho = 13;



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

enum direction
{
  forward,
  backward
};

int sp;
double leftTurnFactor;
double rightTurnFactor;
direction dir;    



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


/*
 *
 * @param speedVal <0, speedUnits>
 * @param turnFactor <0,1>
 * @param speedUnits = specifies the range of speed values passed to argument speedVal
 * @param maxSpeedVal = max effective speed value that is passed to PWM
 * @param minSpeedVal = min effective speed value that is passed to PWM
 */



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

bool hasFoundObstacleOnTheLeft()
{ 
  return leftSensor.isCloserThan(obstacleDistanceDetectionInCm);
}

bool hasFoundObstacleOnTheRight()
{
  return rightSensor.isCloserThan(obstacleDistanceDetectionInCm);
}

bool hasFoundObstacleInFrontOf() 
{
  return frontSensor.isCloserThan(obstacleDistanceDetectionInCm);
}

bool hasFoundObstacle()
{
  return hasFoundObstacleOnTheLeft() || hasFoundObstacleOnTheRight() || hasFoundObstacleInFrontOf();  
}





Steerage car(ENA, ENB, IN1, IN2, IN3, IN4);

void loop()
{
  bool obstacleFound = false;

  Serial.println(leftSensor.getDistanceInCm());
  
  if(obstacleFound == false)
  {
    //input read
    readStateOfController();
    sp = readSpeed();
    dir = readDirection(); 
    
    //set speed of wheels
    car.setSpeedOfLeftWheels(sp, readLeftTurnFactor()); 
    car.setSpeedOfRightWheels(sp, readRightTurnFactor());
    
    //set direction
    if(dir==forward)
      car.straightForward(); //TODO: change name
    else
      car.straightBackward(); //TODO: change name
  }
  else //hasFoundObstacle == true
  {
    car.clearStates(); //stop car //TODO: change it's name
    
    if(hasFoundObstacleOnTheLeft())
    {
      car.omitObstacleOnTheLeft();

    }
    else if(hasFoundObstacleOnTheRight())
    {
      car.omitObstacleOnTheRight();

    }
    else if(hasFoundObstacleInFrontOf()) // obstacles that are in fron of are always get round on the right side 
    {
      car.omitObstacleInFrontOf();
    }
  }
}
