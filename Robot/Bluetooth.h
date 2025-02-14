#ifndef BLUETOOTH_H
#define BLUETOOTH_H


#include "MotorControl.h"

extern MotorControl motorControl;

void getBluetoothData();

extern float targetLatitude;
extern float targetLongitude;

extern bool functionalTestButtonState;

#endif
