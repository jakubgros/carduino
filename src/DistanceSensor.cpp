#include "../includes/DistanceSensor.h"
#include "Arduino.h"

DistanceSensor::DistanceSensor(int triggerPin, int echoPin)
{
    triggerPin_ = triggerPin;
    echoPin_ = echoPin;
}

int DistanceSensor::getDistanceInCm()
{
    clearTriggerPin();
    triggerMeasurement();
    return pulseIn(echoPin_, HIGH) * soundSpeed_ / displacement_;
}

bool DistanceSensor::isCloserThan(int cm)
{
    return getDistanceInCm() < cm;
}

void DistanceSensor::clearTriggerPin()
{
    digitalWrite(triggerPin_, LOW);
    delayMicroseconds(2);
}

void DistanceSensor::triggerMeasurement()
{
    digitalWrite(triggerPin_, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin_, LOW);
}
