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

    void Init();
    void DrawScale(float fraction);
    void DrawFrequency(int frequency);
    void DrawStatus(const char *statusString, bool inverse = false);

public:
    DiyClockDisplay(int frequencyMax, Adafruit_SSD1306 *display);

    void SetFrequency(int frequency);

    void SetStatus(const char *status, bool inverse = false);
};

#endif