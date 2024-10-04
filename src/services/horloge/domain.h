#include "components/board/boardInterface.h"
#include "components/debug/debugInterface.h"
#include "components/network/networkInterface.h"
#include "components/storage/storageInterface.h"
#include "services/horloge/display/displayInterface.h"
#include "components/webserver/webserverinterface.h"
#include "components/ota/otaInterface.h"
#include "components/time/timeInterface.h"
//#include "components/websocket/websocketclientinterface.h"

#define SSIDSTORAGE 10
#define PASSSTORAGE 60

#define APSSID "tamtam"
#define APPWD "T4MT4M"

class Domain {
  private:
    DebugInterface* debug;
    NetworkInterface* network;
    StorageInterface* storage;
    HorlogeDisplayInterface* display;
    WebServerInterface* webServer;
    BoardInterface *board;
    OtaInterface *ota;
    TimeInterface *time;
    //WebsocketClientInterface *websocket;

    // STATE MACHINE
    bool displayWifiAnimation;

    int counter = 0;

    void handleSetupRequest ();
    void handleSaveConfiguration ();
    void handleConfiguration ();
    void handleConfigurationRequest ();
    void handleApiDisplay(bool display);
    void handleSoftReset ();
    void handleOtaProgress (unsigned int progress, unsigned int total);
    void handleWebsocketEvent(uint8_t *payload, size_t size);

  public:
    void setup ();
    void loop ();

    void setBoardInterface(BoardInterface* board);
    void setDebugInterface(DebugInterface* debug);
    void setNetworkInterface(NetworkInterface* network);
    void setStorageInterface(StorageInterface* storage);
    void setDisplayInterface(HorlogeDisplayInterface* display);
    void setWebServerInterface(WebServerInterface* webServer);
    void setOtaInterface(OtaInterface* ota);
    void setTimeInterface(TimeInterface *time);
    //void setWebsocketInterface(WebsocketClientInterface *websocket);
};


const String SETUPSTARTCONTENT = "\
  <html>\
    <head>\
    </head>\
    <body>\
      <h1>Welcome on configuration page of TAMTAM</h1>\
      <form method='POST' action='/conf' enctype='application/x-www-form-urlencoded'>\
        <select name='ssid'>\
";

const String SETUPENDCONTENT = "\
        </select>\
        <input type='text' placeholder='password' name='password' />\
        <input type='submit' value='submit' />\
    </body>\
  </html>\
";

const String CONFIGURATIONCONTENT = "\
  <html>\
    <head>\
    </head>\
    <body>\
      <h1>App configuration of TAMTAM</h1>\
      <form method='GET' action='/reset'>\
        <input type='submit' value='reset configuration' />\
      </form>\
    </body>\
  </html>\
";
