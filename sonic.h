#include <Arduino.h>

namespace Mody {

struct Sonic {
    void setup();

    bool range(long *range);
    bool available() const;
};

}
