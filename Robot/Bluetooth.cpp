#include "Bluetooth.h"

#include <Arduino.h>

void getBluetoothData() {
    static String inputString = "";

    while (Serial2.available()) {
        char inputChar = (char)Serial2.read();
        inputString += inputChar;

        if (inputChar == ';') {
            int spaceIndex = inputString.indexOf(' ');

            if (spaceIndex != -1) {
                String latitude = inputString.substring(0, spaceIndex);
                targetLatitude = latitude.toFloat();

                String longitude = inputString.substring(spaceIndex + 1);
                targetLongitude = longitude.toFloat();

                inputString = "";
            } else {
                inputString = "";
            }
        }

        if (functionalTestButtonState) {
            if (inputChar == 'F') {
                for(int speed = 0; speed <= 255; speed++) {
                    motorControl.moveForward(speed);
                    delay(15);
                }
            }
            if (inputChar == 'B') {
                motorControl.moveBackward(100);
            }
            if (inputChar == 'R') {
                motorControl.turnRight(100);
            }
            if (inputChar == 'L') {
                motorControl.turnLeft(100);
            }
            if (inputChar == 'P') {
                motorControl.stopMotors();
            }
        }
    }
}
