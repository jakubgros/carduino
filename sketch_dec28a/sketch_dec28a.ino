#include "SerialJoystick.h"
#include "Vector.h"
#include "helpers.h"

//ENGINE DRIVER PINOUT
const int ENA =  9; //LEWY
const int IN4 = 2;
const int IN3 = 4;
const int IN2 = 3;
const int IN1 = 5;
const int ENB = 6; //PRAWY

//TODO: calibrate
const int singleOmitMoveLength = 500;
const int lengthOfRotation = 380;

const int controllerMaxValForY = 60;
const int controllerMinValForY = 10;
const int controllerMaxValForX = 60;
const int controllerMinValForX = 10;
const int middlePosForY = controllerMinValForY + (controllerMaxValForY - controllerMinValForY)/2;
const int middlePosForX = controllerMinValForX + (controllerMaxValForX - controllerMinValForX)/2;

Vector controller(middlePosForX, middlePosForY);
SerialJoystick joystick(&Serial);

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
pinMode(ENA, OUTPUT);
pinMode(IN4, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(ENA, OUTPUT);
Serial.begin(9600);
Serial.setTimeout(10);
}

void clearStates()
{
   digitalWrite(IN1, LOW); 
   digitalWrite(IN2, LOW); 
   digitalWrite(IN3, LOW); 
   digitalWrite(IN4, LOW);   
}

void leftForward()
{
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);
}

void rightForward()
{
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH); 
}

void leftBackward()
{
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH); 
}

void rightBackward()
{
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW); 
}

void straightForward()
{
  clearStates();
  rightForward();
  leftForward();
}

void straightBackward()
{
  clearStates();
  rightBackward();
  leftBackward();
}

/*
 *
 * @param speedVal <0, speedUnits>
 * @param turnFactor <0,1>
 * @param speedUnits = specifies the range of speed values passed to argument speedVal
 * @param maxSpeedVal = max effective speed value that is passed to PWM
 * @param minSpeedVal = min effective speed value that is passed to PWM
 */

int calculateSpeed(int speedVal, double turnFactor, int speedUnits = 100, int maxSpeedVal = 250, int minSpeedVal = 80)
{
  int calculatedSpeed;
  if(speedVal == 0)
    calculatedSpeed = 0;
  else
    calculatedSpeed =  minSpeedVal + (1-turnFactor) * speedVal*(maxSpeedVal-minSpeedVal)/speedUnits;
    
  return calculatedSpeed;
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

bool hasFoundObstacleOnTheLeft() //TODO: INPUT
{
  return false;
  //TODO: implement sensor reading
}

bool hasFoundObstacleOnTheRight() //TODO: INPUT
{
  return false;
  //TODO: implement sensor reading
}

bool hasFoundObstacleInFrontOf() //TODO: INPUT
{
  return false;
  //TODO: implement sensor reading
}

bool hasFoundObstacle() //TODO: INPUT
{
  return hasFoundObstacleOnTheLeft() || hasFoundObstacleOnTheRight() || hasFoundObstacleInFrontOf();  
}

void rotateLeftInPlace(int rotateLength)
{  
  clearStates();
  rightForward();
  leftBackward();
  analogWrite(ENA, calculateSpeed(100, 0));
  analogWrite(ENB, calculateSpeed(100, 0));
  delay(rotateLength);
  clearStates();
}

void rotateRightInPlace(int rotateLength)
{
  clearStates();
  rightBackward();
  leftForward();
  analogWrite(ENA, calculateSpeed(100, 0));
  analogWrite(ENB, calculateSpeed(100, 0));
  delay(rotateLength);
  clearStates();
}

void moveForward(int numberOfMovementUnits) //TODO: 
{
  clearStates();
  straightForward();
  delay(numberOfMovementUnits);
  clearStates();
}

void loop()
{
  bool obstacleFound = hasFoundObstacle();



  if(obstacleFound == false)
  {
    //input read
    readStateOfController();
    sp = readSpeed();
    leftTurnFactor = readLeftTurnFactor();
    rightTurnFactor = readRightTurnFactor();
    dir = readDirection(); 
    
    //set speed of wheels
    analogWrite(ENA, calculateSpeed(sp, leftTurnFactor)); 
    analogWrite(ENB, calculateSpeed(sp, rightTurnFactor));
    
    //set direction
    if(dir==forward)
      straightForward(); //TODO: change name
    else
      straightBackward(); //TODO: change name
  }
  else //hasFoundObstacle == true
  {
    clearStates(); //stop car
    
    if(hasFoundObstacleOnTheLeft())
    {
      rotateRightInPlace(lengthOfRotation);
      moveForward(singleOmitMoveLength);
      rotateLeftInPlace(lengthOfRotation);
    }
    else if(hasFoundObstacleOnTheRight())
    {
      rotateLeftInPlace(lengthOfRotation);
      moveForward(singleOmitMoveLength);
      rotateRightInPlace(lengthOfRotation);
    }
    else if(hasFoundObstacleInFrontOf()) // obstacles that are in fron of are always get round on the right side 
    {
      rotateRightInPlace(lengthOfRotation);
      moveForward(singleOmitMoveLength);
      rotateLeftInPlace(lengthOfRotation);
    }
  }
}
