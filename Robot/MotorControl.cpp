#include "MotorControl.h"

#include <Arduino.h>

MotorControl::MotorControl(int leftForwardPin, int leftBackwardPin, int rightForwardPin, int rightBackwardPin, int maxPWM) {
    this->leftForwardPin = leftForwardPin;
    this->leftBackwardPin = leftBackwardPin;
    this->rightForwardPin = rightForwardPin;
    this->rightBackwardPin = rightBackwardPin;
    this->maxPWM = maxPWM;
    this->currentPWM = 1;
}

void MotorControl::automaticControl(float pidOutput) {
    if (pidOutput < -20) {
        pidOutput = map(pidOutput, -180, -20, maxPWM, 70);
        turnLeft(pidOutput);
        currentPWM = 1;
    } else if (pidOutput > 20) {
        pidOutput = map(pidOutput, 20, 180, 70, maxPWM);
        turnRight(pidOutput);
        currentPWM = 1;
    } else {
        digitalWrite(leftBackwardPin, LOW);
        digitalWrite(rightBackwardPin, LOW);

        static unsigned long previousMillis = 0;
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= 10) {
            previousMillis = currentMillis;
            currentPWM += (maxPWM - 1) / (1000 / 10);
            if (currentPWM > maxPWM) {
                currentPWM = maxPWM;
            }
            float adjustment = map(pidOutput, -20, 20, -70, 70);
            int leftPWM = constrain(currentPWM + adjustment, 0, maxPWM);
            int rightPWM = constrain(currentPWM - adjustment, 0, maxPWM);

            digitalWrite(leftBackwardPin, LOW);
            digitalWrite(rightBackwardPin, LOW);
            analogWrite(leftForwardPin, leftPWM);
            analogWrite(rightForwardPin, rightPWM);
        }
    }
}

void MotorControl::stopMotors() {
    digitalWrite(leftBackwardPin, LOW);
    digitalWrite(leftForwardPin, LOW);
    digitalWrite(rightBackwardPin, LOW);
    digitalWrite(rightForwardPin, LOW);
}

void MotorControl::moveForward(int speed) {
    speed = constrain(speed, 0, maxPWM);
    analogWrite(leftForwardPin, speed);
    analogWrite(rightForwardPin, speed);
    digitalWrite(leftBackwardPin, LOW);
    digitalWrite(rightBackwardPin, LOW);
}

void MotorControl::moveBackward(int speed) {
    speed = constrain(speed, 0, maxPWM);
    analogWrite(leftBackwardPin, speed);
    analogWrite(rightBackwardPin, speed);
    digitalWrite(leftForwardPin, LOW);
    digitalWrite(rightForwardPin, LOW);
}

void MotorControl::turnLeft(int speed) {
    speed = constrain(speed, 0, maxPWM);
    analogWrite(leftBackwardPin, speed);
    analogWrite(rightForwardPin, speed);
    digitalWrite(leftForwardPin, LOW);
    digitalWrite(rightBackwardPin, LOW);
}

void MotorControl::turnRight(int speed) {
    speed = constrain(speed, 0, maxPWM);
    analogWrite(leftForwardPin, speed);
    analogWrite(rightBackwardPin, speed);
    digitalWrite(leftBackwardPin, LOW);
    digitalWrite(rightForwardPin, LOW);
}
