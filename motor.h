#include <Arduino.h>

namespace Mody {

struct Motor {
    Motor() : move(STOP)
    { }

    void setup();

    void forward();
    void backward();
    void left();
    void right();
    void stop();

    enum Move {
        STOP = 0,
        FORWARD = 1,
        BACKWARD,
        LEFT,
        RIGHT
    } move;
};

}
