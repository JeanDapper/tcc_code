#include "AzimuthCalculator.h"

#include <math.h>
#include <Arduino.h>

static float degreesToRadians(float numberInDegrees) {
    return numberInDegrees * (PI / 180.0);
}

static float radiansToDegrees(float numberInRadians) {
    return numberInRadians * 180 / PI;
}

float azimuthCalculator(float currentLat, float currentLong, float targetLat, float targetLong) {
    currentLat = degreesToRadians(currentLat);
    currentLong = degreesToRadians(currentLong);
    targetLat = degreesToRadians(targetLat);
    targetLong = degreesToRadians(targetLong);

    float deltaLong = targetLong - currentLong;

    float x = sin(deltaLong) * cos(targetLat);
    float y = cos(currentLat) * sin(targetLat) - sin(currentLat) * cos(targetLat) * cos(deltaLong);

    float azimuth = radiansToDegrees(atan2(x, y));
    return fmod((azimuth + 360), 360);
}
