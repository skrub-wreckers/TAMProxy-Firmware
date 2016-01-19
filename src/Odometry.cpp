#include "Odometry.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {
    Odometer::Odometer(Encoder& lEncoder, Encoder& rEncoder, Gyro& gyro, float alpha)
        : _encL(lEncoder),
          _encR(rEncoder),
          _gyro(gyro),
          _alpha(alpha),
          _lastTime(micros()),
          _lastMeanEnc(0),
          _gyroTot(0) { }

    std::vector<uint8_t> Odometer::handleRequest(std::vector<uint8_t> &request)
    {
        if (request[0] == ODOMETER_READ_CODE) {
            if (request.size() != 1) return {REQUEST_LENGTH_INVALID_CODE};

            // here be dragons
            float vals[] = {_angle, _x, _y};
            uint32_t val = *reinterpret_cast<uint32_t*>(&_angle);

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
        const float ticksPerRev = 3200.0;
        const float wheelRadius = 3.78125 / 2;
        const float baseWidth = 15.3; //inches

        const float ticksPerRad = 3200.0 / (2*M_PI);

        uint32_t lEncVal = _encL.read();
        uint32_t rEncVal = _encR.read();

        // be careful about overflow here
        int32_t diffEnc = lEncVal - rEncVal;
        uint32_t meanEnc = rEncVal + diffEnc/2;

        bool ok;
        float gyroRead = _gyro.read(ok);
        if(!ok) return;

        _gyroTot += gyroRead*(micros() - _lastTime) / 1e6;
        float encAngle = (diffEnc/ticksPerRad) * wheelRadius/baseWidth;
        _angle = _alpha*_gyroTot + (1-_alpha)*encAngle;

        float dr = static_cast<int32_t>(meanEnc - _lastMeanEnc)/ticksPerRev * wheelRadius;
        _x += dr * cos(_angle);
        _y += dr * sin(_angle);

        _lastTime = micros();
        _lastMeanEnc = meanEnc;
    }
}