#include <Arduino.h>

#if CALIBRATION
#include "calibration.h"
#endif

#include "HPGLPlotter.h"

#define BUFFER_SIZE 64
#define DEBUG 1
byte buffer[BUFFER_SIZE];

void processCommand(byte *buffer);

long parseInt(byte *buffer, byte **end);

HPGLPlotter plotter;

void setup() {
#if CALIBRATION
    calibration();
#else
    Serial.begin(9600);
#endif
}

void loop() {
    byte c, i = 0;
    while (Serial.available() != 0) {
        c = (byte) Serial.read();
        if (c <= ' ') {
            continue;
        }

        buffer[i++] = c;

        if (c == ';') {
            processCommand(buffer);
            i = 0;
        }
    }
}

void processCommand(byte *buffer) {
    byte *end;

    switch (buffer[1]) {
        case 'N':
            printf("init\n");
            break;
        case 'P':
            if (buffer[0] == 'I') {
                plotter.boundaries(parseInt(buffer + 2, &end),
                                   parseInt(end + 1, &end),
                                   parseInt(end + 1, &end),
                                   parseInt(end + 1, &end));
            }
            break;
        case 'C':
            plotter.scale(parseInt(buffer + 2, &end),
                          parseInt(end + 1, &end),
                          parseInt(end + 1, &end),
                          parseInt(end + 1, &end));
            break;
        case 'U':
            plotter.penUp();
            break;
        case 'A':
            plotter.plotAbsolute(parseInt(buffer + 2, &end),
                                 parseInt(end + 1, &end));
            break;
        case 'R':
            plotter.plotRelative(parseInt(buffer + 2, &end),
                                 parseInt(end + 1, &end));
            break;
        case 'D':
            plotter.penDown();
            break;
        case ';':
            break;
        default:
            Serial.print("unknown command ");
            Serial.print(buffer[0]);
            Serial.println(buffer[1]);
    }
}

long parseInt(byte *buffer, byte **end) {
    boolean isNegative = false;
    long value = 0;
    byte c;
    int i = 0;

    c = buffer[i];

    do {
        if (c == '-')
            isNegative = true;
        else if (c >= '0' && c <= '9')        // is c a digit?
            value = value * 10 + c - '0';
        i++;
        c = buffer[i];
    }
    while ((c >= '0' && c <= '9'));

    *end = buffer + i;

    if (isNegative)
        value = -value;

    return value;
}