#ifndef FEEDBACKMOTOR_H
#define FEEDBACKMOTOR_H

#include <cstdint>
#include <vector>
#include "Device.h"
#include "TAMPEncoder.h"
#include "Motor.h"
#include "util.h"

// A class that can receive a speed or position value and then use a PID loop to maintain that

namespace tamproxy {

class FeedbackMotor : public Device {
private:
    Encoder& _enc;
	Motor& _motor;

	double _setpoint;
	double _integral;
	double _prevErr;
	double _lastTime;
	void setSetpoint(double val);
public:
    FeedbackMotor(uint8_t dirPin, uint8_t pwmPin);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
    void update();
};

}
#endif