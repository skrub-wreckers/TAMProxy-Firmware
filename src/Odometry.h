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
    float _diffThresh = 0;

    uint32_t _lastTime;
    uint32_t _lastLEncVal;
    uint32_t _lastREncVal;

    float _angle = 0;
    float _omega = 0;
    float _x = 0;
    float _y = 0;
    bool _gyroOk = false;

    // for debug
    float dGyroDt;
    float dEncDt;


public:
    Odometer(Encoder& lEncoder, Encoder& rEncoder, Gyro& gyro, float alpha);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
    void update() override;
};

}
#endif
