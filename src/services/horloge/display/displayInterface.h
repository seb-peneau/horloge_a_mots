#ifndef HorlogeDisplayInterface_h
#define HorlogeDisplayInterface_h

#include "../../../components/debug/debugInterface.h"




class HorlogeDisplayInterface
{
  public:
    virtual void setup (DebugInterface *debug);
    virtual void loop ();

    virtual void displayWifiStatus(bool displayWifiStatus);
};

#endif