#include "DiyClockDisplay.h"

#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Fonts/FreeSans9pt7b.h"

const int SCALE_INSET = 2;
const int SCALE_FRAME_HEIGHT = 10;
const int SCALE_HEIGHT = SCALE_FRAME_HEIGHT - 2 * SCALE_INSET;

const int MARGIN = 4;

#define FREQ_LABEL "Freq: "
#define HZ_LABEL "Hz"
#define STATUS_LABEL "Status: "
#define STATUS_RUNNING "RUN"
#define STATUS_ERROR "ERR"
#define STATUS_HALTED "HALT"

DiyClockDisplay::DiyClockDisplay(int frequencyMax, Adafruit_SSD1306 *display)
{
    _display = display;
    _frequencyMax = frequencyMax;

    _displayWidth = display->width();
    _displayHeight = display->height();

    Init();
}

void DiyClockDisplay::Init()
{
    int16_t x1, y1;
    uint16_t w, h;

    _display->clearDisplay();
    _display->setTextWrap(false);

    _display->setTextColor(SSD1306_WHITE);
    _display->setFont(&FreeSans9pt7b);
    _fontHeight = FreeSans9pt7b.yAdvance;

    // Draw the frequency label
    _display->getTextBounds(FREQ_LABEL, 0, 0, &x1, &y1, &w, &h);
    _fontBaseOffset = _fontHeight - h;

    _frequencyX = w;
    _frequencyBottomY = _fontHeight;

    _display->setCursor(0, _frequencyBottomY - _fontBaseOffset);
    _display->print(FREQ_LABEL);

    // Draw the scale frame
    _scaleFrameY = _frequencyBottomY + MARGIN;
    _display->drawRect(0, _scaleFrameY, _displayWidth, SCALE_FRAME_HEIGHT, SSD1306_WHITE);

    // Draw the status label
    _display->getTextBounds(STATUS_LABEL, 0, 0, &x1, &y1, &w, &h);
    _statusX = w;
    _statusBottomY = _scaleFrameY + SCALE_FRAME_HEIGHT + MARGIN + _fontHeight;
    _display->setCursor(0, _statusBottomY - _fontBaseOffset);
    _display->print(STATUS_LABEL);

    _display->display();
}

void DiyClockDisplay::DrawScale(float fraction)
{
    int scaleWidth = _displayWidth - (2 * SCALE_INSET);

    // Blank out old scale display
    _display->fillRect(SCALE_INSET, _scaleFrameY + SCALE_INSET,
                       scaleWidth, SCALE_HEIGHT,
                       SSD1306_BLACK);

    _display->fillRect(SCALE_INSET, _scaleFrameY + SCALE_INSET,
                       scaleWidth * fraction,
                       SCALE_HEIGHT,
                       SSD1306_WHITE);
}

void DiyClockDisplay::DrawFrequency(int frequency)
{
    _display->fillRect(_frequencyX, 0, _displayWidth, _fontHeight, SSD1306_BLACK);

    _display->setCursor(_frequencyX, _frequencyBottomY - _fontBaseOffset);
    _display->print(frequency);
    _display->print(HZ_LABEL);
}

void DiyClockDisplay::SetFrequency(int frequency)
{
    DrawFrequency(frequency);
    DrawScale(frequency / (float)_frequencyMax);
    _display->display();
}

void DiyClockDisplay::DrawStatus(const char *statusString, bool inverse)
{
    int16_t x1, y1;
    uint16_t w, h;
    _display->getTextBounds(statusString, 0, 0, &x1, &y1, &w, &h);

    _display->fillRect(_statusX, _statusBottomY - _fontHeight, _displayWidth, _fontHeight, SSD1306_BLACK);
    _display->setCursor(_statusX, _statusBottomY - _fontBaseOffset);

    if (inverse)
    {
        _display->fillRect(_statusX, _statusBottomY - _fontHeight, w, _fontHeight, SSD1306_WHITE);
        _display->setTextColor(SSD1306_BLACK);
    }

    _display->print(statusString);

    if (inverse)
    {
        _display->setTextColor(SSD1306_WHITE);
    }
}

void DiyClockDisplay::SetStatus(const char *status, bool inverse)
{
    DrawStatus(status, inverse);
    _display->display();
}