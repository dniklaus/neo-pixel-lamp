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
#include <HsvControl.h>

SerialCommand* sCmd = 0;

// indicator implementation for built in LED
Indicator* led = 0;

HsvControl* hsvControl1 = 0;
HsvControl* hsvControl2 = 0;

void setup()
{
  // setup basic debug environment (heap usage printer, trace ports & dbg cli)
  setupProdDebugEnv();

  // indicator LED
  led = new Indicator("led", "Built in LED.");
  led->assignAdapter(new MyBuiltinLedIndicatorAdapter());

  // Neo Pixels
  const unsigned int cNeoPixelNum1 = 96;
  const unsigned int cNeoPixelPin1 = 7;
  hsvControl1 = new HsvControl(cNeoPixelNum1, cNeoPixelPin1);

  const unsigned int cNeoPixelNum2 = 100;
  const unsigned int cNeoPixelPin2 = 6;
  hsvControl2 = new HsvControl(cNeoPixelNum2, cNeoPixelPin2);
  hsvControl2->setHue(7000);
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  scheduleTimers();         // process Timers
}
