#ifndef HorlogeDisplay_h
#define HorlogeDisplay_h

#include "../displayInterface.h"
#include <NeoPixelBus.h>

class HorlogeDisplay : public HorlogeDisplayInterface
{

  private:
    const uint16_t PixelCount = 66;
    const uint8_t PixelPin = 5;
    NeoPixelBus<NeoRgbwFeature, NeoWs2812xMethod> *strip;
    int repeat  = 0;
    DebugInterface *displayDebug;
    void displayH(int hour);
    void displayM(int minute, int remaining);
    void displayOtaProgress(int progress);
    
  public:
    HorlogeDisplay();
    void setup(DebugInterface *debug);
    void loop ();
    void displayWifiStatus(bool displayWifiStatus);
    void displayConfigurationMode(bool show);
    void clearDisplay();
    void displayHour(int hours, int minutes);
};

#endif