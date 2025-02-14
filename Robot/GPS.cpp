#include "GPS.h"

#include <Arduino.h>

GPSModule::GPSModule() {}

bool GPSModule::getCurrentLocation(float &currentLatitude, float &currentLongitude) {
    if (gps.location.isUpdated()) {
        currentLatitude = gps.location.lat();
        currentLongitude = gps.location.lng();
        return true;
    }
    return false;
}

void GPSModule::smartDelay(unsigned long ms) {
    unsigned long start = millis();
    do {
        while (Serial1.available()) {
            gps.encode(Serial1.read());
        }
    } while (millis() - start < ms);
}
