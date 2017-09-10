#ifndef HPGLDRIVER_CONFIGURATION_H
#define HPGLDRIVER_CONFIGURATION_H

const static long STEPS_PER_MM = 100;

const static long MOTOR_SPEED_X = 600;
const static long MOTOR_SPEED_Y = 600;
const static long MOTOR_SPEED_Z = 400;

const static bool X_DIR_INVERTED = true;
const static long X_STEP = 6;
const static long X_DIR = 7;

const static bool Y_DIR_INVERTED = false;
const static long Y_STEP = 4;
const static long Y_DIR = 5;

const static bool Z_DIR_INVERTED = false;
const static long Z_STEP = 2;
const static long Z_DIR = 3;
const static long Z_CONTACT_STEPS = 100;

const static unsigned char END_SWITCH_PIN = 12;

#define DEBUG 1
#define CALIBRATION 0

#endif //HPGLDRIVER_CONFIGURATION_H
