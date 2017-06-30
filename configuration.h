#ifndef HPGLDRIVER_CONFIGURATION_H
#define HPGLDRIVER_CONFIGURATION_H

const static long STEPS_PER_MM = 100;
const static long MOTOR_SPEED = 3200; // up to 9600; motor has to be in 1/8 step mode

const static unsigned char SERVO_PIN = 10;
const static unsigned char SERVO_UP_POSITION = 70;
const static unsigned char SERVO_DOWN_POSITION = 40;

const static unsigned char END_SWITCH_PIN = 12;

#define DEBUG 0

#endif //HPGLDRIVER_CONFIGURATION_H
