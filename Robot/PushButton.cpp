#include "PushButton.h"

#include <Arduino.h>

PushButton::PushButton(int pin, unsigned long debounceTime)
    : buttonPin(pin), buttonState(LOW), previousState(LOW), 
      lastChangeTime(0), debounceDelay(debounceTime) {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool PushButton::buttonChanged() {
    int reading = digitalRead(buttonPin);

    if (reading != previousState) {
        lastChangeTime = millis();
    }

    if ((millis() - lastChangeTime) > debounceDelay) {
        if (reading != buttonState) {
            buttonState = reading;

            if (buttonState == LOW) {
                previousState = reading;
                return true;
            }
        }
    }

    previousState = reading;
    return false;
}
