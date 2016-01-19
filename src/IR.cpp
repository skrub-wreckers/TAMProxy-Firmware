#include "IR.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {

// Constructor takes a pin number
IR::IR(uint8_t pin) {
    _pin = pin;
    pinMode(pin, INPUT);
    avg = new MovingAverage(10);
}

std::vector<uint8_t> IR::handleRequest(std::vector<uint8_t> &request) {
    if (request[0] == IR_CODE) {
        if (request.size() != 1) return {REQUEST_LENGTH_INVALID_CODE};
        uint16_t val = analogRead(_pin);
        avg.add(val);
        uint16_t output = avg.avg();
        return {static_cast<uint8_t>(output>>8), static_cast<uint8_t>(output)};
    } else  {
        return {REQUEST_BODY_INVALID_CODE};
    }
}

}
