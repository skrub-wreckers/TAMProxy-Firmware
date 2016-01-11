#include "Odometry.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {
    Odometer::Odometer(Encoder& lEncoder, Encoder& rEncoder) : _encL(lEncoder), _encR(rEncoder) {
    }
    
    std::vector<uint8_t> OdometerhandleRequest(std::vector<uint8_t> &request)
    {
        
    }

    void Odometer::update()
    {
        
    }
}