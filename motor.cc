#include "motor.h"

namespace Mody {

enum {
    // right-side wheels
    MOTOR_ENA_PIN = 4,
    MOTOR_IN1_PIN = 5,
    MOTOR_IN2_PIN = 6,
    // left-side wheels
    MOTOR_ENB_PIN = 8,
    MOTOR_IN3_PIN = 9,
    MOTOR_IN4_PIN = 10
};

static void _stop() {
    digitalWrite(MOTOR_ENA_PIN, LOW);
    digitalWrite(MOTOR_ENB_PIN, LOW);
}
static void _go() {
    digitalWrite(MOTOR_ENA_PIN, HIGH);
    digitalWrite(MOTOR_ENB_PIN, HIGH);
}

static void _leftStop() {
    digitalWrite(MOTOR_IN3_PIN, LOW);
    digitalWrite(MOTOR_IN4_PIN, LOW);
}

static void _leftForward() {
    digitalWrite(MOTOR_IN3_PIN, HIGH);
    digitalWrite(MOTOR_IN4_PIN, LOW);
}

static void _leftBackward() {
    digitalWrite(MOTOR_IN3_PIN, LOW);
    digitalWrite(MOTOR_IN4_PIN, HIGH);
}

static void _rightStop() {
    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, LOW);
}

static void _rightForward() {
    digitalWrite(MOTOR_IN1_PIN, HIGH);
    digitalWrite(MOTOR_IN2_PIN, LOW);
}

static void _rightBackward() {
    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, HIGH);
}


void Motor::setup() {
    // configure motor PINs
    pinMode(MOTOR_ENA_PIN, OUTPUT);
    pinMode(MOTOR_IN1_PIN, OUTPUT);
    pinMode(MOTOR_IN2_PIN, OUTPUT);
    pinMode(MOTOR_ENB_PIN, OUTPUT);
    pinMode(MOTOR_IN3_PIN, OUTPUT);
    pinMode(MOTOR_IN4_PIN, OUTPUT);
}

void Motor::forward() {
    if (move == FORWARD) return;
    _stop();
    _leftForward();
    _rightForward();
    _go();
    move = FORWARD;
}

void Motor::backward() {
    if (move == BACKWARD) return;
    _stop();
    _leftBackward();
    _rightBackward();
    _go();
    move = BACKWARD;
}

void Motor::left() {
    if (move == LEFT) return;
    _stop();
    _leftForward();
    _rightBackward();
    _go();
    move = LEFT;
}

void Motor::right() {
    if (move == RIGHT) return;
    _stop();
    _leftBackward();
    _rightForward();
    _go();
    move = RIGHT;
}

void Motor::stop() {
    _stop();
    _leftStop();
    _rightStop();
    move = STOP;
}

}
