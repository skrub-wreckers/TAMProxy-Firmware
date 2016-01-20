#ifndef IR_H
#define IR_H

#include <cstdint>
#include <vector>
#include "Device.h"
#include "Util.h"

namespace tamproxy {

// For long and short IR, not ultrashort ones
class IR : public Device {
private:
    uint8_t _pin;
    MovingAverage* avg;
public:
    IR(uint8_t pin);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

}
#endif
