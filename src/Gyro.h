#ifndef GYRO_H
#define GYRO_H

#include <cstdint>
#include <vector>
#include "Device.h"

namespace tamproxy {

// For the ADXRS450
class Gyro : public Device {
private:
  uint8_t _sspin;
  static const uint32_t CLOCK_SPEED = 2000000; // Clocked at 2 MHz
  static const uint32_t READ_WORD = 0x20000000;
public:
  Gyro(uint8_t sspin);
  int16_t read(bool &ok);
  static float toRadians(int16_t angle);
  std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

}

#endif
