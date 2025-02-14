#ifndef PREAMBLE_H
#define PREAMBLE_H

#include <Wire.h>
#include <MicroLCD.h>

#include "GPS.h"
#include "PID.h"
#include "Compass.h"
#include "Bluetooth.h"
#include "PushButton.h"
#include "MotorControl.h"
#include "AzimuthCalculator.h"

#define rightBackwardPin 8
#define rightForwardPin 9
#define leftBackwardPin 10
#define leftForwardPin 11


#define maxPWM 255

#define motorButtonPin 2
#define functionalTestButtonPin 3
#define ledPin 5

#endif
