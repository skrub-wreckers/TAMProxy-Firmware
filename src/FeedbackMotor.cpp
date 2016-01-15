#include "FeedbackMotor.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"
#include "util.h"

const double kP = 1;
const double kI = 0;
const double kD = 0;

namespace tamproxy {

// Constructor takes a motor and its encoder
FeedbackMotor::FeedbackMotor() {
    setSetpoint(0);
}

// Either sets new setpoint (speed or dist) or tells to stop updating
std::vector<uint8_t> FeedbackMotor::handleRequest(std::vector<uint8_t> &request) {
    if (request[0] == FEEDBACK_MOTOR_CODE) {
        if (request.size() != 4) return {REQUEST_LENGTH_INVALID_CODE};
            digitalWrite(_dirPin, request[1] != 0);
            analogWrite(_pwmPin, (request[2]<<8) + request[3]);
            return {OK_CODE};
    } else {
        return {REQUEST_BODY_INVALID_CODE};
    }
}

void setSetpoint(double val) {
    _setpoint = val;
    _integral = 0;
    _prevErr = 0;
    _lastTime = micros();
}

// PID loop for Speed
void FeedbackMotor::update() {
    uint32_t encVal = _enc.read();
    uint32_t elapsedTime = micros() - _lastTime;

    uint32_t err = setpoint - encVal;
    integral += err * elapsedTime;
    derivative = (err - prevErr) / elapsedTime;
    prevErr = err;
    double power = kP * err + kI * integral + kD * derivative;

    _motor.
    
    _gyroTot += _gyro.read()*(micros()-_lastTime); //Get the right number out of _gyro.read()
    float lDist = (_encL.read()/3200)*3.78125*3.14159; //Magic number = wheel diameter
    float rDist = (_encR.read()/3200)*3.78125*3.14159;
    float encAngle = 360*(lDist - rDist)/53.407; // 53.407" = Turn IP circumference
    
    _angle = (_alpha*_gyroTot)+((1-_alpha)*encAngle);
    
    _lastTime = micros();
}

}
