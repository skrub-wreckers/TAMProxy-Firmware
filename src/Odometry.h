#ifndef ODOMETRY_H
#define ODOMETRY_H

#include <cstdint>
#include <vector>
#include "Device.h"
#include "Encoder.h"

namespace tamproxy {

class Odometer : public Device {
private:
    Encoder& _encL;
    Encoder& _encR;
    Gyroscope& _gyro;

    uint32_t _lastTime;

    uint32_t _gyroTot;
    uint32_t _encTot;

    uint32_t _angle;

    float _alpha; //Mixing weight for the gyroscope

public:
    Odometer(Encoder& lEncoder, Encoder& rEncoder, Gyroscope& gyro, float alpha);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
    void update();
};

}
#endif
