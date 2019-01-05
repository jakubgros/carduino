#include "DistanceSensor.h"

#define trigPin 9
#define echoPin 8

DistanceSensor ds(trigPin, echoPin);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println("distance sensor start");
}

void loop()
{
  int distance = ds.getDistanceInCm();
  
  Serial.print("Distance: ");
  Serial.println(distance);
  
  Serial.print("is closer than 20cm: ");
  Serial.println(ds.isCloserThan(20));
}
