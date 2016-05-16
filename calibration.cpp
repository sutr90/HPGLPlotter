#include "calibration.h"
#include "AccelStepper.h"
#include <avr/sleep.h>
#include <Arduino.h>

void block() {
    while (Serial.available() == 0) { }
}

void runMotor(AccelStepper &stepper, int distance) {
    long start = stepper.currentPosition();
    stepper.runToNewPosition(start + 100);
    Serial.println("end reached. going back");
    stepper.runToNewPosition(start);
    Serial.println("back at start again");
}

void testRun(AccelStepper &stepper, int distance) {
    runMotor(stepper, distance);
    Serial.println("Please input mms traveled.");
    block();
    long mils = Serial.parseInt();
    float steps_in_mm = (float) distance / mils;
    Serial.print("To go 1mm you have to make ");
    Serial.print(steps_in_mm);
    Serial.print(" steps.");

}

void calibration() {
    Serial.begin(9600);
    Serial.println("This is calibration routine for HPGL Plotter");
    Serial.println("The motor attached to pins 2,3,4,5");
    Serial.println("will be moved 100, 200 and 300 steps");
    Serial.println("in positive direction and then back to original location.");

    AccelStepper s1(AccelStepper::HALF4WIRE, 2, 4, 3, 5);

    testRun(s1, 100);
    testRun(s1, 200);
    testRun(s1, 300);

    s1.disableOutputs();

    Serial.println("Calibration finished. Going to sleep now.");

    delay(100);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
}
