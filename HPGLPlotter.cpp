#include "HPGLPlotter.h"

const float HPGLPlotter::UNITS_PER_MM = 40.f;
//TODO: move to config
const float HPGLPlotter::STEPS_PER_MM = 15;

HPGLPlotter::HPGLPlotter() : s1(AccelStepper::HALF4WIRE, 2, 4, 3, 5), s2(AccelStepper::HALF4WIRE, 8, 10, 9, 11), ms(),
                             s() {
    s1.setMaxSpeed(100);
    s2.setMaxSpeed(100);

    ms.addStepper(s1);
    ms.addStepper(s2);
    s.attach(6);
}

void HPGLPlotter::init() {
    penUp();
    goHome(s1);
    goHome(s2);
}

void HPGLPlotter::goHome(AccelStepper &stepper) {
    while (!endSwitch()) {
        stepper.move(-5);
    }
    stepper.move(5);
    stepper.setCurrentPosition(0);
}

void HPGLPlotter::boundaries(long x1, long y1, long x2, long y2) {
    P1X = x1;
    P1Y = y1;
    P2X = x2;
    P2Y = y2;
}

void HPGLPlotter::scale(long x1, long y1, long x2, long y2) {
    scaleX = (P1X - P2X) / (float) (x1 - x2);
    scaleY = (P1Y - P2Y) / (float) (y1 - y2);
}

void HPGLPlotter::penUp() {
    s.write(0);
}

void HPGLPlotter::penDown() {
    s.write(135);
}

void HPGLPlotter::plotAbsolute(long x, long y) {
    //input in user units
    position[0] = convertUserUnitsToSteps(x, scaleX);
    position[1] = convertUserUnitsToSteps(y, scaleY);
    updateMotors();
}

void HPGLPlotter::plotRelative(long deltaX, long deltaY) {
    //input in user units
    position[0] += convertUserUnitsToSteps(deltaX, scaleX);
    position[1] += convertUserUnitsToSteps(deltaY, scaleY);
    updateMotors();
}

bool HPGLPlotter::endSwitch() {
    //TODO: define proper end switch
    return digitalRead(12) == HIGH;
}

void HPGLPlotter::updateMotors() {
    // position in steps
    ms.moveTo(position);
    ms.runSpeedToPosition();
}

long HPGLPlotter::convertUserUnitsToSteps(long user_units, float scale) {
    // user units * scale = plotter units / UNITS_PER_MM => millimeters
    //FIXME: move /40 to scale directly?
    float mils = (user_units * scale) / UNITS_PER_MM;
    return (long) (mils * STEPS_PER_MM);
}
