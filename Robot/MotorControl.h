#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

class MotorControl {
  public:
    MotorControl(int leftForwardPin, int leftBackwardPin, int rightForwardPin, int rightBackwardPin, int maxPWM);

    void automaticControl(float pidOutput);
    void stopMotors();

    void moveForward(int speed); 
    void moveBackward(int speed);
    void turnLeft(int speed);     
    void turnRight(int speed);    

  private:
    int leftForwardPin, leftBackwardPin, rightForwardPin, rightBackwardPin;
    int maxPWM;
    float currentPWM;
};

#endif
