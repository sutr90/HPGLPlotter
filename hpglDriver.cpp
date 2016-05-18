#include <Arduino.h>

#define DEBUG 1

#if CALIBRATION
#include "calibration.h"
#endif

#include "HPGLPlotter.h"

#define BUFFER_SIZE 64

char buffer[BUFFER_SIZE];

void processCommand(char *buffer);

long parseInt(char *buffer, char **end);

HPGLPlotter plotter;

void setup() {
#if CALIBRATION
    calibration();
#else
    Serial.begin(9600);
    Serial.println("This is HPGL Plotter 1.0a");
#endif
}

char c, i = 0;

void loop() {

    if(Serial.available() != 0) {
        c = (char) Serial.read();

#if DEBUG
        Serial.println(c);
#endif
        if (c <= ' ') {
            return;
        }

        buffer[i++] = c;

        if (c == ';') {
#if DEBUG
            Serial.print("Buffer contains: ");
            Serial.println(buffer);
#endif
            processCommand(buffer);
            i = 0;
        }
    }
}

void processCommand(char *buffer) {
    char *end;

    switch (buffer[1]) {
        case 'N':
            plotter.init();
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
            Serial.println(buffer);
    }
}

long parseInt(char *buffer, char **end) {
    boolean isNegative = false;
    long value = 0;
    char c;
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