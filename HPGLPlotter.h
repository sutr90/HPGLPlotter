//
// Created by VF on 15. 5. 2016.
//

#ifndef HPGLDRIVER_FUNCTIONS_H
#define HPGLDRIVER_FUNCTIONS_H


#include <Servo.h>
#include <MultiStepper.h>
#include <AccelStepper.h>

class HPGLPlotter {
private:
    long position[2];

    void goHome(AccelStepper &stepper);

    long P1X, P1Y, P2X, P2Y;

    static const float UNITS_PER_MM;
    static const long STEPS_PER_MM;
    float scaleX = 1.0f;
    float scaleY = 1.0f;

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
    void boundaries(long x1, long y1, long x2, long y2);
    void scale(long x1, long y1, long x2, long y2);
    void penUp();
    void penDown();
    void plotAbsolute(long x, long y);
    void plotRelative(long deltaX, long deltaY);

    void resetPositons();

    void shutdown();
};

#endif //HPGLDRIVER_FUNCTIONS_H
