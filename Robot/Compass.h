#ifndef COMPASS_H
#define COMPASS_H

#include <Arduino.h>

class Compass {
public:
    enum CompassType { UNDEFINED, HMC5883, QMC5883 };

    Compass(CompassType type);
    bool initialize();
    CompassType getType() const;
    void setDeclination(int degrees, int minutes, char direction);
    float readDirection();
    void startCalibration();
    void endCalibration();

private:
    static const int HMC5883_ADDRESS = 0x1E;
    static const int HMC5883_MODE_REGISTER = 2;
    static const int HMC5883_X_HIGH_REGISTER = 3;
    static const int HMC5883_STATUS_REGISTER = 9;

    static const int QMC5883_ADDRESS = 0x0D;
    static const int QMC5883_CONFIG_REGISTER = 9;
    static const int QMC5883_SET_RESET_REGISTER = 11;
    static const int QMC5883_X_LOW_REGISTER = 0;
    static const int QMC5883_STATUS_REGISTER = 6;

    int16_t minX, minY, maxX, maxY;
    float scaleX = 1.0;
    float scaleY = 1.0;
    int16_t offsetX = 0;
    int16_t offsetY = 0;

    int address;
    CompassType type;

    float declinationAngle = -0.34976; // Jaraguá do Sul (rad)

    void requestData(int statusRegister, int dataRegister);
};

#endif
