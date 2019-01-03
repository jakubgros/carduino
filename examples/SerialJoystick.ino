#include "includes/SerialJoystick.h"
#include "includes/Vector.h"
#include "includes/helpers.h"

Vector direction(15, 15);
SerialJoystick joystick(&Serial1);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  Serial1.begin(9600);
  Serial1.setTimeout(10);
  Serial.println("uartStart");
  pinMode(2, OUTPUT);
}

void loop()
{
  joystick.readPositionIfAvailable(&direction);
  flush(&Serial1);
  Serial.print(direction.x);
  Serial.print(",");
  Serial.print(direction.y);
  Serial.println();

}
