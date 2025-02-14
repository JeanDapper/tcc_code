#ifndef PID_H
#define PID_H

class PID {
  public:
    PID(float Kp, float Ki, float Kd);

    float compute(float targetValue, float measuredValue);

  private:
    float normalizeAngle(float angle);

    float Kp, Ki, Kd;

    float previousError;
    float integral;
    unsigned long lastTime;
};

#endif
