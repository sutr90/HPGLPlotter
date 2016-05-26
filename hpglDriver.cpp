#include <Arduino.h>

#define DEBUG 0

#if CALIBRATION
#include "calibration.h"
#endif

#include "HPGLPlotter.h"

#define BUFFER_SIZE 64

char buffer[BUFFER_SIZE];

void processCommand(char *buffer);

long parseLong(char *buffer, char **end);

HPGLPlotter plotter;

void setup() {
#if CALIBRATION
    calibration();
#else
    Serial.begin(9600);
    Serial.println("This is HPGL Plotter 1.0a");
#endif

    Serial.print("READY$");
}

char c, i = 0;
long x1, x2, y1, y2;

void loop() {

    if (Serial.available() != 0) {
        c = (char) Serial.read();

#if DEBUG
        Serial.println(c);
#endif
        if (c <= ' ') {
            return;
        }

        buffer[i++] = c;

        if (c == ';') {
            processCommand(buffer);
            plotter.shutdown();
            delay(300);
            Serial.print("ACK$");
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
                x1 = parseLong(buffer + 2, &end);
                y1 = parseLong(end + 1, &end);
                x2 = parseLong(end + 1, &end);
                y2 = parseLong(end + 1, &end);
                plotter.boundaries(x1, y1, x2, y2);
            }
            break;
        case 'C':
            x1 = parseLong(buffer + 2, &end);
            y1 = parseLong(end + 1, &end);
            x2 = parseLong(end + 1, &end);
            y2 = parseLong(end + 1, &end);
            plotter.scale(x1, y1, x2, y2);
            break;
        case 'U':
            plotter.penUp();
            break;
        case 'A':
            x1 = parseLong(buffer + 2, &end);
            y1 = parseLong(end + 1, &end);
            plotter.plotAbsolute(x1, y1);
            break;
        case 'R':
            x1 = parseLong(buffer + 2, &end);
            y1 = parseLong(end + 1, &end);
            plotter.plotRelative(x1, y1);
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

long parseLong(char *buffer, char **end) {
    boolean isNegative = false;
    long value = 0;
    char c;
    int i = 0;

    c = buffer[i];

    do {
        if (c == '-')
            isNegative = true;
        else if (c >= '0' && c <= '9')
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