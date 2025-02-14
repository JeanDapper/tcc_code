#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

class PushButton {
private:
    int buttonPin;
    int buttonState;
    int previousState;
    unsigned long lastChangeTime;
    const unsigned long debounceDelay;

public:
    PushButton(int pin, unsigned long debounceTime = 50);

    bool buttonChanged();
};

#endif
