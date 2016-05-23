#include "HPGLPlotter.h"

#define DEBUG 0

const float HPGLPlotter::UNITS_PER_MM = 40.f;
//TODO: move to config
const long HPGLPlotter::STEPS_PER_MM = 333;

HPGLPlotter::HPGLPlotter() : s1(AccelStepper::HALF4WIRE, 2, 4, 3, 5), s2(AccelStepper::HALF4WIRE, 8, 10, 9, 11), ms(),
                             s() {

    s1.setMaxSpeed(1000);
    s2.setMaxSpeed(1000);

    s1.setAcceleration(1000.0);
    s1.setSpeed(1000.0);

    s2.setAcceleration(1000.0);
    s2.setSpeed(1000.0);

    ms.addStepper(s1);
    ms.addStepper(s2);

    //setup endswitch pins
    pinMode(12, INPUT_PULLUP);
}

void HPGLPlotter::init() {
#if DEBUG
    Serial.println("init");
#endif
    penUp();
    goHome(s1);
    goHome(s2);
    resetPositons();
}

void HPGLPlotter::goHome(AccelStepper &stepper) {
    //move to zero until end switch is hit
    while(!endSwitch()){
        stepper.move(-STEPS_PER_MM);
        stepper.setSpeed(200);
        while(stepper.distanceToGo() > 0){
            stepper.runSpeedToPosition();
        }
    }

    //back off a bit
    stepper.move(STEPS_PER_MM);
    stepper.setSpeed(200);
    while(stepper.distanceToGo() > 0){
        stepper.runSpeedToPosition();
    }
}

void HPGLPlotter::boundaries(long x1, long y1, long x2, long y2) {
#if DEBUG
    Serial.print("boundaries");
    Serial.print(x1);
    Serial.print(",");
    Serial.print(y1);
    Serial.print(",");
    Serial.print(x2);
    Serial.print(",");
    Serial.println(y2);
#endif

    P1X = x1;
    P1Y = y1;
    P2X = x2;
    P2Y = y2;
}

void HPGLPlotter::scale(long x1, long y1, long x2, long y2) {
#if DEBUG
    Serial.print("scale");
    Serial.print(x1);
    Serial.print(",");
    Serial.print(y1);
    Serial.print(",");
    Serial.print(x2);
    Serial.print(",");
    Serial.println(y2);
#endif

    scaleX = (P1X - P2X) / (float) (x1 - x2);
    scaleY = (P1Y - P2Y) / (float) (y1 - y2);
}

void HPGLPlotter::penUp() {
#if DEBUG
    Serial.println("penup");
#endif
    //TODO: replace with config pin
    s.attach(10);
    s.write(0);
    delay(150);
    s.detach();
}

void HPGLPlotter::penDown() {
#if DEBUG
    Serial.println("pendown");
#endif
    //TODO: replace with config pin
    s.attach(10);
    s.write(75);
    delay(150);
    s.detach();
}

void HPGLPlotter::plotAbsolute(long x, long y) {
#if DEBUG
    Serial.print("plotAbsolute");
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
#endif
    //input in user units
    position[0] = convertUserUnitsToSteps(x, scaleX);
    position[1] = convertUserUnitsToSteps(y, scaleY);
    updateMotors();
}

void HPGLPlotter::plotRelative(long deltaX, long deltaY) {
#if DEBUG
    Serial.print("plotAbsolute");
    Serial.print(deltaX);
    Serial.print(",");
    Serial.println(deltaY);
#endif
    //input in user units
    position[0] += convertUserUnitsToSteps(deltaX, scaleX);
    position[1] += convertUserUnitsToSteps(deltaY, scaleY);
    updateMotors();
}

bool HPGLPlotter::endSwitch() {
    //TODO: define proper end switch
    return digitalRead(12) == LOW;
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

void HPGLPlotter::resetPositons() {
    s1.setCurrentPosition(0);
    s2.setCurrentPosition(0);

    position[0] = 0;
    position[1] = 0;
}


