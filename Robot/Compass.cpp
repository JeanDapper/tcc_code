#include "Compass.h"

#include <Wire.h>

Compass::Compass(CompassType type) {
    this->type = type;
}

bool Compass::initialize() {
    if (type == UNDEFINED) {
        Wire.beginTransmission(HMC5883_ADDRESS);
        if (Wire.endTransmission() == 0) {
            type = HMC5883;
            address = HMC5883_ADDRESS;
        } else {
            Wire.beginTransmission(QMC5883_ADDRESS);
            if (Wire.endTransmission() == 0) {
                type = QMC5883;
                address = QMC5883_ADDRESS;
            }
        }
    }

    if (type == HMC5883) {
        Wire.beginTransmission(address);
        Wire.write(HMC5883_MODE_REGISTER);
        Wire.write(0x00);  
        Wire.endTransmission();
    } else if (type == QMC5883) {
        Wire.beginTransmission(address);
        Wire.write(QMC5883_SET_RESET_REGISTER);
        Wire.write(0x01); 
        Wire.endTransmission();
        Wire.beginTransmission(address);
        Wire.write(QMC5883_CONFIG_REGISTER);
        Wire.write(0x0D); 
        Wire.endTransmission();
    }

    return type != UNDEFINED;
}

Compass::CompassType Compass::getType() const {
    return type;
}

void Compass::setDeclination(int degrees, int minutes, char direction) {
    declinationAngle = (degrees + minutes / 60.0) * PI / 180.0;
    if (direction == 'W') {
        declinationAngle = -declinationAngle;
    }
    Serial.println(declinationAngle);
}

float Compass::readDirection() {
    int16_t x, y, z;

    if (type == UNDEFINED) {
        return 0.0;
    }

    if (type == HMC5883) {
        requestData(HMC5883_STATUS_REGISTER, HMC5883_X_HIGH_REGISTER);
        x = Wire.read() << 8 | Wire.read();
        z = Wire.read() << 8 | Wire.read();
        y = Wire.read() << 8 | Wire.read();
    } else if (type == QMC5883) {
        requestData(QMC5883_STATUS_REGISTER, QMC5883_X_LOW_REGISTER);
        x = Wire.read() | Wire.read() << 8;
        y = Wire.read() | Wire.read() << 8;
        z = Wire.read() | Wire.read() << 8;
    }

    if (x < minX) minX = x;
    if (x > maxX) maxX = x;
    if (y < minY) minY = y;
    if (y > maxY) maxY = y;

    float correctedX = (x - offsetX) * scaleX;
    float correctedY = (y - offsetY) * scaleY;
    float angle = atan2(correctedY, correctedX) + declinationAngle;

    if (angle < 0) {
        angle += 2.0 * PI;
    } else if (angle > 2.0 * PI) {
        angle -= 2.0 * PI;
    }

    return (angle * 180.0) / PI;
}

void Compass::requestData(int statusRegister, int dataRegister) {
    do {
        Wire.beginTransmission(address);
        Wire.write(statusRegister);
        Wire.endTransmission();
        Wire.requestFrom(address, 1);
    } while ((Wire.read() & 1) == 0);

    Wire.beginTransmission(address);
    Wire.write(dataRegister);
    Wire.endTransmission();
    Wire.requestFrom(address, 6);
}

void Compass::startCalibration() {
    maxX = maxY = -32768;
    minX = minY = 32767;
}

void Compass::endCalibration() {
    Serial.print("X: ");
    Serial.print(minX);
    Serial.print(" - ");
    Serial.println(maxX);
    Serial.print("Y: ");
    Serial.print(minY);
    Serial.print(" - ");
    Serial.println(maxY);

    offsetX = (maxX + minX) / 2;
    offsetY = (maxY + minY) / 2;

    int16_t varX = maxX - minX;
    int16_t varY = maxY - minY;
    if (varY == varX) {
        scaleX = scaleY = 1.0;
    } else if (varY > varX) {
        scaleY = 1.0;
        scaleX = static_cast<float>(varY) / varX;
    } else {
        scaleX = 1.0;
        scaleY = static_cast<float>(varX) / varY;
    }
}
