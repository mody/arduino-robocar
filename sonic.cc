#include "sonic.h"

namespace Mody {

enum {
    ECHO_PIN = 2,
    TRIG_PIN = 12,
};

enum Status {
    IDLE = 0,
    STARTED,
    LISTENING,
};

struct State {
    State() : start(0L), diff(0L), status(IDLE)
    { }

    long start;
    long diff;
    Status status;
};


volatile static State state;
static long range = 0;

ISR (PCINT0_vect) {
    int level = PIND & _BV(ECHO_PIN);
    long us = micros();

    if (level && state.status == STARTED) {
        state.start = us;
        state.status = LISTENING;
    } else if (!level && state.status == LISTENING) {
        state.diff = us - state.start;
        state.status = IDLE;
    } else {
        // noop
    }
}


static void ping() {
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    state.status = STARTED;
}


void Sonic::setup() {
    // setup sonic
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);

    // clear trigger
    digitalWrite(TRIG_PIN, LOW);

    state.status = IDLE;

    attachInterrupt(INT0, PCINT0_vect, CHANGE);
}


bool Sonic::range(long *range) {
    if (state.status == IDLE) {
        ping();
    }
    if (range) {
        *range = state.diff;
    }
    return !!state.diff;
}


bool Sonic::available() const {
    return !!state.diff;
}


}
