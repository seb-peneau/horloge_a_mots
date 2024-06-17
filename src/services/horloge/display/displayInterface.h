#ifndef HorlogeDisplayInterface_h
#define HorlogeDisplayInterface_h

#include "../../../components/debug/debugInterface.h"




class HorlogeDisplayInterface
{
  public:
    virtual void setup (DebugInterface *debug);
    virtual void loop ();

    virtual void displayWifiStatus(bool displayWifiStatus);
    virtual void displayConfigurationMode(bool show);
    virtual void clearDisplay();
    virtual void displayHour(int hours, int minutes);
    virtual void displayOtaProgress(int progress);
};

#endif