#include "Odometry.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {
    Odometer::Odometer(Encoder& lEncoder, Encoder& rEncoder, Gyro& gyro, float alpha) : _encL(lEncoder), _encR(rEncoder), _gyro(gyro), _alpha(alpha), _lastTime(micros())   {
    }

    std::vector<uint8_t> Odometer::handleRequest(std::vector<uint8_t> &request)
    {
        if (request[0] == ODOMETER_READ_CODE) {
            if (request.size() != 1) return {REQUEST_LENGTH_INVALID_CODE};

            uint32_t val = static_cast<uint32_t>(_angle);
            return {
                static_cast<uint8_t>(val>>24),
                static_cast<uint8_t>(val>>16),
                static_cast<uint8_t>(val>>8),
                static_cast<uint8_t>(val)
            };
        }
        else {
            return {REQUEST_BODY_INVALID_CODE};
        }
    }

    void Odometer::update()
    {
        uint32_t lEncVal = _encL.read();
        uint32_t rEncVal = _encR.read();

        _gyroTot += _gyro.read()*(micros()-_lastTime); //Get the right number out of _gyro.read()
        float lDist = (_encL.read()/3200)*3.78125*3.14159; //Magic number = wheel diameter
        float rDist = (_encR.read()/3200)*3.78125*3.14159;
        float encAngle = 360*(lDist - rDist)/53.407; // 53.407" = Turn IP circumference

        _angle = (_alpha*_gyroTot)+((1-_alpha)*encAngle);

        _lastTime = micros();
    }
}