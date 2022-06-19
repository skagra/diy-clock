
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "DiyClockDisplay.h"
#include <TimerOne.h>
#include "Button.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

#define FREQ_PIN A7
#define RUN_STOP_PIN PIN2
#define CLOCK_PIN 10
#define FREQ_MAX 100

#define MIN_ANALOGUE_DELTA 20

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DiyClockDisplay *clockDisplay;

float analogFreqScale = FREQ_MAX / (float)1024;
Button runStopButton(RUN_STOP_PIN, callback, (void *)0);
bool running = false;

unsigned long period;

void setup()
{
    Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    // Set up display
    display.clearDisplay();
    clockDisplay = new DiyClockDisplay(FREQ_MAX, &display);

    // PWN clock pulse
    Timer1.initialize();

    // Clock speed pot
    pinMode(A7, INPUT);
}

void callback(void *clientData)
{
    running = !running;

    if (running)
    {
        Timer1.pwm(CLOCK_PIN, 512, period);
        clockDisplay->SetStatus("RUN");
    }
    else
    {
        Timer1.stop();
        digitalWrite(CLOCK_PIN, LOW);
        clockDisplay->SetStatus("HALT");
    }
}

int currentAnalogValue = 10000;

void loop()
{

    int analogReading = analogRead(FREQ_PIN);
    if (abs(analogReading - currentAnalogValue) > MIN_ANALOGUE_DELTA)
    {
        currentAnalogValue = analogReading;

        int freq = currentAnalogValue * analogFreqScale + 1;
        clockDisplay->SetFrequency(freq);

        period = (float)1000000 / freq;

        if (running)
        {
            Timer1.pwm(CLOCK_PIN, 512, period);
        }
    }
    runStopButton.tick();
}
