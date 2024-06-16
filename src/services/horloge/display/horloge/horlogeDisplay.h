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
    

  public:
    HorlogeDisplay();
    void setup(DebugInterface *debug);
    void loop ();
    void displayWifiStatus(bool displayWifiStatus);
};

#endif