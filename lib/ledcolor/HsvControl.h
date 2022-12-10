#pragma once

class SpinTimer;
class Adafruit_NeoPixel;

class HsvControl
{
public:
  HsvControl(unsigned int neoPixelNum, unsigned int neoPixelPin);
  virtual ~HsvControl();

  void process();
  void setHue(unsigned short hue);

private:
  unsigned short m_hue;
  bool m_isHueRunning;
  unsigned char m_saturation;
  unsigned char m_value;
  SpinTimer* m_timer;
  Adafruit_NeoPixel* m_strip;
};
