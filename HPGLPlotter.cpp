#include "configuration.h"
#include "HPGLPlotter.h"

const float HPGLPlotter::UNITS_PER_MM = 40.f;

// AccelStepper::DRIVER, stepPin, directionPin
HPGLPlotter::HPGLPlotter() : s1(AccelStepper::DRIVER, 2, 3),
                             s2(AccelStepper::DRIVER, 5, 6),
                             ms(), s() {
    s1.setEnablePin(4);
    s2.setEnablePin(7);

    s1.setPinsInverted(true, false, true);
    s2.setPinsInverted(false, false, true);

    s1.setMaxSpeed(MOTOR_SPEED);
    s2.setMaxSpeed(MOTOR_SPEED);
    s1.setAcceleration(MOTOR_SPEED*3);
    s2.setAcceleration(MOTOR_SPEED*3);

    ms.addStepper(s1);
    ms.addStepper(s2);

    pinMode(END_SWITCH_PIN, INPUT_PULLUP);
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
#if DEBUG
    Serial.println("go home");
#endif
    //move to zero until end switch is hit
    while (!endSwitch()) {
#if DEBUG
        Serial.println("homing");
#endif
        stepper.move(-STEPS_PER_MM / 2);
        stepper.setSpeed(1200);
        while (stepper.distanceToGo() != 0) {
            if (endSwitch()) {
                break;
            }
            stepper.runSpeedToPosition();
        }
    }

    //back off a bit
    stepper.move(2*STEPS_PER_MM);
    stepper.setSpeed(1200);
    while (stepper.distanceToGo() != 0) {
        stepper.runSpeedToPosition();
    }

    stepper.disableOutputs();
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
#if DEBUG
    Serial.print("scale");
    Serial.print(scaleX);
    Serial.print(",");
    Serial.println(scaleY);
#endif
}

void HPGLPlotter::penUp() {
#if DEBUG
    Serial.println("penup");
#endif
    s.attach(SERVO_PIN);
    s.write(SERVO_UP_POSITION);
    delay(150);
    s.write(SERVO_UP_POSITION - SERVO_RELIEF); // back off a bit to release the tension
    delay(150);
    s.detach();
}

void HPGLPlotter::penDown() {
#if DEBUG
    Serial.println("pendown");
#endif
    s.attach(SERVO_PIN);
    s.write(SERVO_DOWN_POSITION);
    delay(150);
    s.detach();
}

void HPGLPlotter::plotAbsolute(long x, long y) {
    //input in user units
    position[0] = convertUserUnitsToSteps(x, scaleX);
    position[1] = convertUserUnitsToSteps(y, scaleY);

#if DEBUG
    Serial.print("plotAbsolute");
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
    Serial.print(position[0]);
    Serial.print(",");
    Serial.println(position[1]);
#endif

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
    return digitalRead(END_SWITCH_PIN) == LOW;
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

void HPGLPlotter::disable() {
    s1.disableOutputs();
    s2.disableOutputs();
}

void HPGLPlotter::enable() {
    s1.enableOutputs();
    s2.enableOutputs();
}
