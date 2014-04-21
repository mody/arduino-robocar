#include <Arduino.h>

#include "motor.h"
#include "sonic.h"

// PIN mapping
// /usr/share/arduino/hardware/arduino/variants/standard/pins_arduino.h

// limit 30cm
enum { RANGE_LIMIT = 30 };

Mody::Motor motor;
Mody::Sonic sonic;

void setup() {
    Serial.begin(9600);

    motor.setup();
    sonic.setup();
}


void loop() {
    static long range = 0;
    if (sonic.range(&range)) {
        range /= 58; // convert to cm
    }

    if (!Serial.available()) {
        return;
    }

    // handle communication
    const byte in = Serial.read();

    switch(in) {
    case 'F': // forward
    case 'f': // forward
    case '1': // forward
        if (!range || range >= RANGE_LIMIT) {
            motor.forward();
        } else {
            motor.stop();
        }
        break;
    case 'B': // backward
    case 'b': // backward
    case '2': // backward
        motor.backward(); break;
    case 'L': // turn left
    case 'l': // turn left
    case '3': // turn left
        motor.left(); break;
    case 'R': // turn right
    case 'r': // turn right
    case '4': // turn right
        motor.right(); break;
    default:
        motor.stop();
    }
}

