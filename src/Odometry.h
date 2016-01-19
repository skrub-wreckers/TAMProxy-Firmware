#ifndef ODOMETRY_H
#define ODOMETRY_H

#include <cstdint>
#include <vector>
#include "Device.h"
#include "Encoder.h"
#include "Gyro.h"

namespace tamproxy {

class Odometer : public Device {
private:
    Encoder& _encL;
    Encoder& _encR;
    Gyro& _gyro;
    float _alpha = 0; //Mixing weight for the gyroscope

    uint32_t _lastTime;
    uint32_t _lastMeanEnc;

    float _gyroTot;

    float _angle = 0;
    float _x = 0;
    float _y = 0;

public:
    Odometer(Encoder& lEncoder, Encoder& rEncoder, Gyro& gyro, float alpha);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
    void update();
};

}
#endif
