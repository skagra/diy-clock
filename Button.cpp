#include "Button.h"

Button::Button(byte buttonPin, void (*callback)(void *), void *clientData)
{
    _buttonPin = buttonPin;
    pinMode(_buttonPin, INPUT);
    _callback = callback;
    _clientData = clientData;
}

void Button::tick()
{
    unsigned long now = millis();
    if (now >= _lastPressedMillis + _DEBOUNCE_DELAY_MILLIS)
    {
        if (digitalRead(_buttonPin) == HIGH && !_currentlyPressed)
        {
            _currentlyPressed = true;
            _lastPressedMillis = now;
            _callback(_clientData);
        }
        else
        {
            if (digitalRead(_buttonPin) == LOW && _currentlyPressed)
            {
                _currentlyPressed = false;
            }
        }
    }
}