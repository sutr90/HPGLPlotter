#include "calibration.h"
#include <AccelStepper.h>
#include <avr/sleep.h>

void block() {
    Serial.flush();
    Serial.readString();
    while (Serial.available() == 0) { }
}

void runMotor(AccelStepper &stepper, int distance) {
    stepper.setCurrentPosition(0);
    stepper.moveTo(distance);
    stepper.setMaxSpeed(1000.0);
    stepper.setAcceleration(1000.0);
    stepper.setSpeed(1000.0);
    while (stepper.distanceToGo() != 0) {
        stepper.runSpeedToPosition();
    }
}

void testRun(AccelStepper &stepper, int distance) {
    Serial.println("Send S to start");
    block();
    stepper.enableOutputs();

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
    stepper.disableOutputs();
}

void calibration() {
    Serial.begin(115200);
    Serial.println("This is calibration routine for HPGL Plotter");
    Serial.println("The motor attached to pins 2,3,4,5");
    Serial.println("will be moved 100, 200 and 300 steps");
    Serial.println("in positive direction and then back to original location.");

    AccelStepper stepper(AccelStepper::HALF4WIRE, 2, 4, 3, 5);

    testRun(stepper, 3000);
    testRun(stepper, 2000);

    Serial.println("Calibration finished. Going to sleep now.");

    delay(100);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
}
