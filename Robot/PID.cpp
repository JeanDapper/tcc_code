#include "PID.h"

#include <Arduino.h>

PID::PID(float Kp, float Ki, float Kd) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;

    previousError = 0.0;
    integral = 0.0;
    lastTime = 0;
}

float PID::compute(float targetValue, float measuredValue) {
    float error = 0.0, derivative = 0.0, output = 0.0;
    unsigned long currentTime = millis();
    float timeChange = (currentTime - lastTime) / 1000.0;

    error = normalizeAngle(targetValue - measuredValue);

    integral += error * timeChange;

    derivative = (error - previousError) / timeChange;

    output = Kp * error + Ki * integral + Kd * derivative;

    output = constrain(output, -180, 180);

    previousError = error;
    lastTime = currentTime;

    return output;
}

float PID::normalizeAngle(float angle) {
    while (angle > 180) angle -= 360;
    while (angle < -180) angle += 360;
    return angle;
}
