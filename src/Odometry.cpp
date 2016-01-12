#include "Odometry.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {
    Odometer::Odometer(Encoder& lEncoder, Encoder& rEncoder, Gyroscope& gyro, float alpha) : _encL(lEncoder), _encR(rEncoder), _gyro(gyro), _alpha(alpha), _lastTime(micros())  {
    }

    std::vector<uint8_t> Odometer::handleRequest(std::vector<uint8_t> &request)
    {

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