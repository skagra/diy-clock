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
#define SINGLE_STEP_PIN PIN3
#define RESET_PIN PIN4
#define CLOCK_PIN 10
#define FREQ_MAX 100
#define PULSE_MICROS 5000
#define MIN_ANALOGUE_DELTA 20
#define DISPLAY_PAUSE_MILLIS 500

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DiyClockDisplay *clockDisplay;

float analogFreqScale = FREQ_MAX / (float)1024;
Button runStopButton(RUN_STOP_PIN, runStopCallback, (void *)0);
Button singleStepButton(SINGLE_STEP_PIN, singleStepCallback, (void *)0);
Button resetButton(RESET_PIN, resetCallback, (void *)0);

bool running = false;
int currentAnalogValue = 10000;
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
   pinMode(FREQ_PIN, INPUT);

   // Reset
   reset();
}

void halt()
{
   Timer1.stop();
   running = false;
   digitalWrite(CLOCK_PIN, LOW);
   clockDisplay->setStatus(DiyClockDisplay::Status::Halt);
}

void run()
{
   Timer1.pwm(CLOCK_PIN, 512, period);
   clockDisplay->setStatus(DiyClockDisplay::Status::Run);
   running = true;
}

void reset()
{
   halt();

   clockDisplay->setStatus(DiyClockDisplay::Status::Reset);
   delay(DISPLAY_PAUSE_MILLIS);

   // TODO - Set reset line high

   digitalWrite(CLOCK_PIN, LOW);
   delayMicroseconds(PULSE_MICROS);
   digitalWrite(CLOCK_PIN, HIGH);
   delayMicroseconds(PULSE_MICROS);
   digitalWrite(CLOCK_PIN, LOW);
   delayMicroseconds(PULSE_MICROS);

   // TODO - Reset reset line

   clockDisplay->setStatus(DiyClockDisplay::Status::Halt);
}

void resetCallback(void *clientData)
{
   reset();
}

void singleStepCallback(void *clientData)
{
   if (running)
   {
      clockDisplay->showError();
   }
   else
   {
      clockDisplay->setStatus(DiyClockDisplay::Status::Step);
      digitalWrite(CLOCK_PIN, LOW);
      delayMicroseconds(PULSE_MICROS);
      digitalWrite(CLOCK_PIN, HIGH);
      delayMicroseconds(PULSE_MICROS);
      digitalWrite(CLOCK_PIN, LOW);
      delayMicroseconds(PULSE_MICROS);
      delay(DISPLAY_PAUSE_MILLIS);
      clockDisplay->setStatus(DiyClockDisplay::Status::Halt);
   }
}

void runStopCallback(void *clientData)
{
   if (!running)
   {
      run();
   }
   else
   {
      halt();
   }
}

void loop()
{
   int analogReading = analogRead(FREQ_PIN);
   if (abs(analogReading - currentAnalogValue) > MIN_ANALOGUE_DELTA)
   {
      currentAnalogValue = analogReading;

      int freq = currentAnalogValue * analogFreqScale + 1;
      clockDisplay->setFrequency(freq);

      period = (float)1000000 / freq;

      if (running)
      {
         Timer1.pwm(CLOCK_PIN, 512, period);
      }
   }
   runStopButton.tick();
   singleStepButton.tick();
   resetButton.tick();
}
