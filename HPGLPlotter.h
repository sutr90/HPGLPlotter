#ifndef HPGLDRIVER_FUNCTIONS_H
#define HPGLDRIVER_FUNCTIONS_H

#include <MultiStepper.h>
#include <AccelStepper.h>

class HPGLPlotter {
private:
    long position[2];

    void goHome(AccelStepper &stepper, const char *name);

    long P1X, P1Y, P2X, P2Y;

    static const float UNITS_PER_MM;
    float scaleX = 1.0f;
    float scaleY = 1.0f;

    MultiStepper multiStepper;
    AccelStepper stepperX;
    AccelStepper stepperY;
    AccelStepper stepperZ;

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

    void disable();
    void enable();
};

#endif //HPGLDRIVER_FUNCTIONS_H
