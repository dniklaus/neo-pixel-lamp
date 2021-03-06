/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>

// PlatformIO libraries
#include <SerialCommand.h>  // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <SpinTimer.h>      // pio lib install 1699, lib details see https://github.com/dniklaus/wiring-timer
#include <Adafruit_NeoPixel.h>


// private libraries
#include <ProductDebug.h>

// local components (lib folder)
#include <Indicator.h>
#include <MyBuiltinLedIndicatorAdapter.h>

SerialCommand* sCmd = 0;

// indicator implementation for built in LED
Indicator* led = 0;

// 
const unsigned int cAnalogPin = A0;  // potentiometer wiper (middle terminal) connected to analog pin 0
                            // outside leads to ground and +5V

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
const unsigned int cNeoPixelPin = 6;
const unsigned int cNeoPixelNum = 16;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(cNeoPixelNum, cNeoPixelPin, NEO_GRB + NEO_KHZ800);

// Read Neo Pixel Color value from ADC
uint32_t readColorPreset()
{
  uint32_t val = analogRead(cAnalogPin);  // read the input pin
  val = val * 255;
  val = val / 1023;
  return val;
}

// Fill the dots with one color
void fillOneColor(uint32_t c)
{
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void setup()
{
  // setup basic debug environment (heap usage printer, trace ports & dbg cli)
  setupProdDebugEnv();

  // indicator LED
  led = new Indicator("led", "Built in LED.");
  led->assignAdapter(new MyBuiltinLedIndicatorAdapter());

  strip.begin();
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'
  fillOneColor(strip.Color(255, 255, 255));
}

void loop()
{
  strip.setBrightness(readColorPreset());
  strip.show();

  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  scheduleTimers();         // process Timers
}
