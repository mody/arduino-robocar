#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "motor.h"
#include "sonic.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


// PIN mapping
// /usr/share/arduino/hardware/arduino/variants/standard/pins_arduino.h

// limit 30cm
enum { RANGE_STOP_LIMIT = 20, RANGE_GO_LIMIT = 35 };

Mody::Motor motor;
Mody::Sonic sonic;

void setup() {
    Serial.begin(9600);

    lcd.begin(20,4);
    lcd.backlight();
    lcd.clear();

    motor.setup();
    sonic.setup();
}

static bool stop_range(const long range) {
    return range && range <= RANGE_STOP_LIMIT;
}

static bool go_range(const long range) {
    return range && range >= RANGE_GO_LIMIT;
}

static bool is_path_clear() {
    static long range = 0;
    static bool clear = true;

    if (sonic.range(&range)) {
        range /= 58; // convert to cm
    }

    if (stop_range(range)) {
        clear = false;
    } else if (!clear && go_range(range)) {
        clear = true;
    }

    return clear;
}

void loop() {
    const bool clear_path = is_path_clear();

    if (!Serial.available()) {
        return;
    }

    // handle communication
    const byte in = Serial.read();

    switch(in) {
    case 'F': // forward
    case 'f': // forward
    case '1': // forward
        if (clear_path) {
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

