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

            std::vector<uint8_t> res(3*4 + 1);
            size_t i = 0;
            for(float f : {_angle, _x, _y}) {
                // here be dragons
                uint32_t val = *reinterpret_cast<uint32_t*>(&f);
                res[i++] = static_cast<uint8_t>(val>>24);
                res[i++] = static_cast<uint8_t>(val>>16);
                res[i++] = static_cast<uint8_t>(val>>8);
                res[i++] = static_cast<uint8_t>(val);
            }
            res[i++] = _gyroOk;
            return res;
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
        const float ticksPerRad = ticksPerRev / (2*M_PI);

        uint32_t lEncVal = _encL.read();
        uint32_t rEncVal = _encR.read();

        // be careful about overflow here
        int32_t diffEnc = rEncVal - lEncVal;
        uint32_t meanEnc = lEncVal + diffEnc/2;

        float encAngle = (diffEnc/ticksPerRad) * wheelRadius/baseWidth;

        // Use the gyro, if possible
        int16_t rawGyro = _gyro.read(_gyroOk);
        uint32_t currTime = micros();

        if(_gyroOk) {
            float gyroRead = Gyro::toRadians(rawGyro);

            _gyroTot += gyroRead*(currTime - _lastTime) / 1e6;
            _angle = _alpha*_gyroTot + (1-_alpha)*encAngle;
        }
        else {
            _angle = encAngle;
        }

        float dr = static_cast<int32_t>(meanEnc - _lastMeanEnc)/ticksPerRad * wheelRadius;
        _x += dr * cos(_angle);
        _y += dr * sin(_angle);

        _lastTime = currTime;
        _lastMeanEnc = meanEnc;
    }
}