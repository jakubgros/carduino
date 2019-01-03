#ifndef DistanceSensor_h
#define DistanceSensor_h

class DistanceSensor
{
public:
    DistanceSensor(int triggerPin, int echoPin);
    int getDistanceInCm();
    bool isCloserThan(int cm);

private:
    void clearTriggerPin();
    void triggerMeasurement();

private:
    int triggerPin_;
    int echoPin_;
    const double soundSpeed_ = 0.034;
    const double displacement_ = 2;
};

#endif