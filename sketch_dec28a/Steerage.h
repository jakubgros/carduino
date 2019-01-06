#ifndef Steerage_h
#define Steerage_h

#include "helpers.h"

class Steerage
{
    //TODO: calibrate
const int singleOmitMoveLength = 500;
const int lengthOfRotation = 380;

    const int ENA;
    const int ENB;
    const int IN1;
    const int IN2;
    const int IN3;
    const int IN4;

    void leftForward();
    void rightForward();
    void leftBackward();
    void rightBackward();

    void rotateLeftInPlace(int rotateLength);
    void rotateRightInPlace(int rotateLength);
    void moveForward(int numberOfMovementUnits); //TODO: change it's name because now it's confusing in combination with straightForward

    public:
    Steerage(const int ENA, const int ENB, const int IN1, const int IN2, const int IN3, const int IN4):
    ENA(ENA), ENB(ENB), IN1(IN1), IN2(IN2), IN3(IN3), IN4(IN4)
    {
    }

    void setSpeedOfLeftWheels(int speedVal, double turnFactor, int speedUnits = 100, int maxSpeedVal = 250, int minSpeedVal = 80);
    void setSpeedOfRightWheels(int speedVal, double turnFactor, int speedUnits = 100, int maxSpeedVal = 250, int minSpeedVal = 80);

    /*
 *
 * @param speedVal <0, speedUnits>
 * @param turnFactor <0,1>
 * @param speedUnits = specifies the range of speed values passed to argument speedVal
 * @param maxSpeedVal = max effective speed value that is passed to PWM
 * @param minSpeedVal = min effective speed value that is passed to PWM
 */
    int calculateSpeed(int speedVal, double turnFactor, int speedUnits = 100, int maxSpeedVal = 250, int minSpeedVal = 80);
    
    void clearStates();
    void omitObstacleOnTheLeft();
    void omitObstacleOnTheRight();
    void omitObstacleInFrontOf();

    void setDirection(direction dir);
};

#endif