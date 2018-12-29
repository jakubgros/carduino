//PINOUT
const int ENA =  9; //LEWY
const int IN4 = 2;
const int IN3 = 4;
const int IN2 = 3;
const int IN1 = 5;
const int ENB = 6; //PRAWY

enum direction
{
  forward,
  backward
};

void setup() 
{
pinMode(ENA, OUTPUT);
pinMode(IN4, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(ENA, OUTPUT);
Serial.begin(9600);
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
    calculatedSpeed =  minSpeedVal + (speedVal*(1-turnFactor))*(maxSpeedVal-minSpeedVal)/speedUnits;
    
  Serial.println(calculatedSpeed);
  return calculatedSpeed;
}

// @@@@@@@@@@@@@@@ INPUTS @@@@@@@@@@@@@@@@@@@@@@@@@
int sp;
double leftTurnFactor;
double rightTurnFactor;
direction dir;       

int readSpeed() //TODO:
{
  
  const int MaxChars = 5; // an int string contains up to 5 digits and
                          // is terminated by a 0 to indicate end of string
  char strValue[MaxChars+1]; // must be big enough for digits and terminating null
  int index = 0;         // the index into the array storing the received digits

  int speedVal = 0;
  while(true)
  {
    char ch = Serial.read();
    if(ch >= '0' && ch <= '9') // is this an ascii digit between 0 and 9?
       speedVal = (speedVal * 10) + (ch - '0'); // yes, accumulate the value
    else if (ch == 10)  // is the character the newline character
    {
       Serial.println(speedVal);
       break;
    }
  }
  return speedVal;
}

double readLeftTurnFactor() //TODO:
{
  return 1; //output val <0;1>
}

double readRightTurnFactor() //TODO:
{
  return 0; //output (val <0;1> 
}

direction readDirection() //TODO:
{
  return direction::forward;
}

void loop()
{
  //input read
  sp = readSpeed();
  leftTurnFactor = readLeftTurnFactor(); 1; 
  rightTurnFactor = readRightTurnFactor();
  dir = readDirection(); 
  
  //set speed of wheels
  analogWrite(ENA, calculateSpeed(sp, leftTurnFactor));
  analogWrite(ENB, calculateSpeed(sp, rightTurnFactor));
  
  //set direction
  if(dir==forward)
    straightForward();
  else
    straightBackward();
}
