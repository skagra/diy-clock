#ifndef _DIY_CLOCK_DISPLAY
#define _DIY_CLOCK_DISPLAY

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

class DiyClockDisplay
{
private:
    Adafruit_SSD1306 *_display;
    int16_t _displayWidth;
    int16_t _displayHeight;

    int _frequencyMax;

    int _fontHeight;
    int _fontBaseOffset;

    int _frequencyX;
    int _frequencyBottomY;

    int _scaleFrameY;

    int _statusX;
    int _statusBottomY;

    void init();
    void drawScale(float fraction);
    void drawFrequency(int frequency);
    void drawStatus(const char *statusString, bool inverse = false);

public:
    enum Status
    {
        Halt,
        Run,
        Reset,
        Step
    };

    DiyClockDisplay(int frequencyMax, Adafruit_SSD1306 *display);

    void setFrequency(int frequency);

    void setStatus(Status status);

    void showError();
};

#endif