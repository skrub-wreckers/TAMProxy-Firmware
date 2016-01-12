#include "Odometry.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {
    Odometer::Odometer(Encoder& lEncoder, Encoder& rEncoder, Gyroscope& gyro) : _encL(lEncoder), _encR(rEncoder), _gyro(gyro) {
    }
    
    std::vector<uint8_t> OdometerhandleRequest(std::vector<uint8_t> &request)
    {
        
    }

    void Odometer::update()
    {
        
    }
}