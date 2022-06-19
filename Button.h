#ifndef _BUTTON_DOT_H_
#define _BUTTON_DOT_H_

#include <Arduino.h>

class Button
{
private:
    static const unsigned long _DEBOUNCE_DELAY_MILLIS = 100;

    byte _buttonPin;
    void (*_callback)(void *);
    void *_clientData;
    unsigned long _lastPressedMillis = 0;
    bool _currentlyPressed = false;

public:
    Button(byte buttonPin, void (*callback)(void *), void *clientData);
    void tick();
};

#endif