#include "HsvControl.h"
#include <SpinTimer.h>
#include <Adafruit_NeoPixel.h>


//-----------------------------------------------------------------------------

class MySpinTimerAction : public SpinTimerAction
{
private:
  HsvControl* m_hsvCtrl;
public:
  MySpinTimerAction(HsvControl* hsvCtrl)
  : m_hsvCtrl(hsvCtrl)
  { }

  void timeExpired()
  {
    if (0 != m_hsvCtrl)
    {
      m_hsvCtrl->process();
    }
  }
};

//-----------------------------------------------------------------------------

const unsigned char HsvControl::c_defaultBrightness = 200;

HsvControl::HsvControl(unsigned int neoPixelNum, unsigned int neoPixelPin, unsigned char brightness /* = c_defaultBrightness*/)
: m_hue(0)
, m_isHueRunning(true)
, m_saturation(255)
, m_value(255)
, m_timer(new SpinTimer(100, new MySpinTimerAction(this), SpinTimer::IS_RECURRING, SpinTimer::IS_AUTOSTART))
, m_strip(new Adafruit_NeoPixel(neoPixelNum, neoPixelPin, NEO_GRB + NEO_KHZ800))
{ 
  m_strip->begin();
  m_strip->setBrightness(brightness);
  m_strip->clear();
  m_strip->show();
}

HsvControl::~HsvControl()
{ 
  delete m_strip;
  m_strip = 0;

  delete m_timer->action();
  m_timer->attachAction(0);

  delete m_timer;
  m_timer = 0;
}

void HsvControl::process()
{
  if (m_isHueRunning)
  {
    m_hue += 50;
  }

  if (0 != m_strip)
  {
    unsigned long int rgbColor = m_strip->gamma32(m_strip->ColorHSV(m_hue, m_saturation, m_value));
    m_strip->fill(rgbColor);
    m_strip->show();
  }
}

void HsvControl::setHue(unsigned short hue)
{
  m_isHueRunning = false;
  m_hue = hue;
}

void HsvControl::setSaturation(unsigned char saturation)
{
  m_saturation = saturation;
}
