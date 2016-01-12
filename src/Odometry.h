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

public:
    Odometer(Encoder& lEncoder, Encoder& rEncoder, Gyroscope& gyro);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
    void update();
};

}
#endif
