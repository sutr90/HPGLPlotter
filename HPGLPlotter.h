//
// Created by VF on 15. 5. 2016.
//

#ifndef HPGLDRIVER_FUNCTIONS_H
#define HPGLDRIVER_FUNCTIONS_H


#include <Servo/src/Servo.h>
#include "MultiStepper.h"
#include "AccelStepper.h"

class HPGLPlotter {
private:
    long position[2];

    void goHome(AccelStepper &stepper);

    int P1X, P1Y, P2X, P2Y;

    static const float UNITS_PER_MM;
    static const float STEPS_PER_MM;
    float scaleX;
    float scaleY;

    Servo s;
    MultiStepper ms;
    AccelStepper s1;
    AccelStepper s2;

    bool endSwitch();
    void updateMotors();
    long convertUserUnitsToSteps(long user_units, float scale);
public:
    HPGLPlotter();
    void init();
    void boundaries(int x1, int y1, int x2, int y2);
    void scale(int x1, int y1, int x2, int y2);
    void penUp();
    void penDown();
    void plotAbsolute(long x, long y);
    void plotRelative(long deltaX, long deltaY);
};

#endif //HPGLDRIVER_FUNCTIONS_H
