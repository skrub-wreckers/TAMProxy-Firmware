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
          _lastLEncVal(0),
          _lastREncVal(0)
        { }

    std::vector<uint8_t> Odometer::handleRequest(std::vector<uint8_t> &request)
    {
        if (request[0] == ODOMETER_READ_CODE) {
            if (request.size() != 1) return {REQUEST_LENGTH_INVALID_CODE};

            std::vector<uint8_t> res(5*4 + 1);
            size_t i = 0;
            for(float f : {_angle, _x, _y, dGyroDt, dEncDt}) {
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

        // calculate wheel angular changes since last run
        float dThetaL = static_cast<int32_t>(lEncVal - _lastLEncVal)/ticksPerRad;
        float dThetaR = static_cast<int32_t>(rEncVal - _lastREncVal)/ticksPerRad;

        // change in robot angle as estimated by encoders
        float dAngleEnc = (dThetaR - dThetaL) * wheelRadius/baseWidth;

        // Use the gyro, if possible
        int16_t rawGyro = _gyro.read(_gyroOk);
        uint32_t currTime = micros();
        double dt = (currTime - _lastTime) / 1e6;


        float dAngle;
        if(_gyroOk) {
            dGyroDt = Gyro::toRadians(rawGyro);
            float dAngleGyro = dGyroDt * dt;

            // smoothing for derivative - http://stackoverflow.com/q/1023860/102441
            const float dencTau = 0.0025;
            float dencAlpha = exp(-dt / dencTau);
            dEncDt = dencAlpha * dEncDt + (1-dencAlpha) * (dAngleEnc / dt);

            //http://www-personal.umich.edu/~johannb/Papers/paper63.pdf
            float diffDt = abs(dGyroDt - dEncDt);
            if(diffDt > _alpha)
                dAngle = dAngleGyro;
            else
                dAngle = dAngleEnc;
        }
        else {
            dAngle = dAngleEnc;
        }

        // update estimates of position and angle
        float dr = (dThetaR + dThetaL)/2 * wheelRadius;
        _x += dr * cos(_angle + dAngle/2);
        _y += dr * sin(_angle + dAngle/2);
        _angle += dAngle;

        _lastTime = currTime;
        _lastLEncVal = lEncVal;
        _lastREncVal = rEncVal;
    }
}
