#include "configuration.h"
#include "HPGLPlotter.h"

const float HPGLPlotter::UNITS_PER_MM = 40.f;

// AccelStepper::DRIVER, stepPin, directionPin
HPGLPlotter::HPGLPlotter() : stepperX(AccelStepper::DRIVER, X_STEP, X_DIR),
                             stepperY(AccelStepper::DRIVER, Y_STEP, Y_DIR),
                             stepperZ(AccelStepper::DRIVER, Z_STEP, Z_DIR),
                             multiStepper() {
    stepperX.setPinsInverted(X_DIR_INVERTED, false, true);
    stepperY.setPinsInverted(Y_DIR_INVERTED, false, true);
    stepperZ.setPinsInverted(Z_DIR_INVERTED, false, true);

    stepperX.setSpeed(MOTOR_SPEED_X);
    stepperX.setMaxSpeed(MOTOR_SPEED_X);
    stepperY.setSpeed(MOTOR_SPEED_Y);
    stepperY.setMaxSpeed(MOTOR_SPEED_Y);
    stepperZ.setSpeed(MOTOR_SPEED_Z);
    stepperZ.setMaxSpeed(MOTOR_SPEED_Z);

    multiStepper.addStepper(stepperX);
    multiStepper.addStepper(stepperY);

    pinMode(END_SWITCH_PIN, INPUT_PULLUP);
}

void HPGLPlotter::init() {
#if DEBUG
    Serial.println("init");
#endif
    // Z has to be initialized first, to lift the pen from the paper
    goHome(stepperZ, "Z");
    goHome(stepperX, "X");
    goHome(stepperY, "Y");
    resetPositons();
}

void HPGLPlotter::goHome(AccelStepper &stepper, const char *name) {
#if DEBUG
    Serial.println("go home");
#endif
    //move to zero until end switch is hit
    while (!endSwitch()) {
#if DEBUG
        Serial.print("homing ");
        Serial.println(name);
#endif
        stepper.move(-STEPS_PER_MM / 2);
        stepper.setSpeed(400); // this is important!! move() resets the current speed
        while (stepper.distanceToGo() != 0) {
            if (endSwitch()) {
                break;
            }
            stepper.runSpeedToPosition();
        }
    }

    //back off a bit
    stepper.move(2 * STEPS_PER_MM);
    stepper.setSpeed(400);
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
    stepperZ.moveTo(0);
    stepperZ.setSpeed(MOTOR_SPEED_Z);
    while (stepperZ.distanceToGo() != 0) {
        stepperZ.runSpeedToPosition();
    }
}

void HPGLPlotter::penDown() {
#if DEBUG
    Serial.println("pendown");
#endif
    stepperZ.moveTo(Z_DOWN_POSITION);
    stepperZ.setSpeed(MOTOR_SPEED_Z);
    while (stepperZ.distanceToGo() != 0) {
        stepperZ.runSpeedToPosition();
    }
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
    multiStepper.moveTo(position);
    multiStepper.runSpeedToPosition();
}

long HPGLPlotter::convertUserUnitsToSteps(long user_units, float scale) {
    // user units * scale = plotter units / UNITS_PER_MM => millimeters
    //FIXME: move /40 to scale directly?
    float mils = (user_units * scale) / UNITS_PER_MM;
    return (long) (mils * STEPS_PER_MM);
}

void HPGLPlotter::resetPositons() {
    stepperX.setCurrentPosition(0);
    stepperY.setCurrentPosition(0);
    stepperZ.setCurrentPosition(0);

    position[0] = 0;
    position[1] = 0;
}

void HPGLPlotter::disable() {
    stepperX.disableOutputs();
    stepperY.disableOutputs();
    stepperZ.disableOutputs();
}

void HPGLPlotter::enable() {
    stepperX.enableOutputs();
    stepperY.enableOutputs();
    stepperZ.enableOutputs();
}
