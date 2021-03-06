#include "configuration.h"
#include "calibration.h"
#include <AccelStepper.h>
#include <avr/sleep.h>
#include <Servo.h>

void block() {
    Serial.flush();
    Serial.readString();
    while (Serial.available() == 0) {}
}

void runMotor(AccelStepper &stepper, int distance) {
    stepper.enableOutputs();

    stepper.setCurrentPosition(0);
    stepper.moveTo(distance);
    stepper.setMaxSpeed(10000.0);
    stepper.setAcceleration(10000.0);
    stepper.setSpeed(400.0);
    while (stepper.distanceToGo() != 0) {
        stepper.runSpeedToPosition();
    }
    stepper.disableOutputs();
}

void testRun(AccelStepper &stepper, int distance) {
    Serial.println("Send S to start");
    block();

    runMotor(stepper, distance);

    Serial.println("end reached");
    Serial.println("Please input mms traveled.");
    block();
    long mils = Serial.parseInt();
    float steps_in_mm = (float) distance / mils;
    Serial.print("To go 1mm you have to make ");
    Serial.print(steps_in_mm);
    Serial.print(" steps.");

    runMotor(stepper, -distance);
    Serial.println("back at start again");
}

void calibration() {
    Serial.begin(9600);
    Serial.println("This is calibration routine for HPGL Plotter");
#if CALIBRATION == 2
    Servo servo1;
    Serial.println("The servo is attached to pin 9");
    servo1.attach(9);
    while (true) {
        Serial.println("input the desired servo position:");
        block();
        long v = Serial.parseInt();
        servo1.write(v);
        delay(15);
    }
#else
    Serial.println("The motor attached to pins 6, 7");
    Serial.println("will be moved 1000, 2000 and 3000 steps");
    Serial.println("in positive direction and then back to original location.");

    AccelStepper stepper(AccelStepper::DRIVER, 6, 7);

    testRun(stepper, 1000);
    testRun(stepper, 2000);
    testRun(stepper, 3000);
#endif
    Serial.println("Calibration finished. Going to sleep now.");

    delay(100);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
}
