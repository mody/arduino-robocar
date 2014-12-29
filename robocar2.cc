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
    const long now = micros();

    if (sonic.range(&range)) {
        range /= 58; // convert to cm
    }

    if ((now % 10000) == 0) {
        char text[20];
        snprintf(text, 20, "I see %ldcm %20c", range, 32);
        lcd.setCursor(0, 0);
        lcd.print(text);
    }

    if (stop_range(range)) {
        clear = false;
    } else if (!clear && go_range(range)) {
        clear = true;
    }

    return clear;
}

void loop() {
    static byte last_cmd = 0;

    const bool clear_path = is_path_clear();

    if (!Serial.available()) {
        return;
    }

    // handle communication
    const byte in = Serial.read();

    if (in != last_cmd) {
        last_cmd = in;
        char text[20];
        snprintf(text, 20, "Command: %c %20c", in, 32);
        lcd.setCursor(0, 1);
        lcd.print(text);
    }

    switch(in) {
    case 'F': // forward
        if (clear_path) {
            motor.forward();
        } else {
            motor.stop();
        }
        break;
    case 'B': // backward
        motor.backward(); break;
    case 'L': // turn left
        motor.left(); break;
    case 'R': // turn right
        motor.right(); break;
    default:
        motor.stop();
    }
}

