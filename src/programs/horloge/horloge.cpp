
#include <Arduino.h>

#include "./services/horloge/domain.h"

#include "./components/debug/console/console.h"
#include "./components/network/wifi/wifi.h"
#include "./components/storage/eeprom/storeEeprom.h"
#include "./components/board/ESP/espBoard.h"
#include "./components/storage/spiff/storeSpiff.h"
#include "./services/horloge/display/horloge/horlogeDisplay.h"
#include "./components/webserver/webserver/webserver.h"
#include "./components/ota/espOta/espOta.h"
#include "./components/time/ntp/ntp.h"

Domain* Tamtam = new Domain();

void setup() {
  Console* console = new Console();
  HorlogeDisplayInterface *ledDisplay = new HorlogeDisplay(); // Specific display for horloge
  Tamtam->setBoardInterface(new ESPBoard()); // for ESP based board
  Tamtam->setDebugInterface(console);
  Tamtam->setDisplayInterface(ledDisplay);
  Tamtam->setNetworkInterface(new EspWifi());
  Tamtam->setStorageInterface(new StoreEeprom()); // USE EEPROM for storage
  // Tamtam->setStorageInterface(new StoreSpiff()); // USE SPIFF for storage
  Tamtam->setWebServerInterface(new EspWebServer());
  Tamtam->setOtaInterface(new ESPOta());
  Tamtam->setTimeInterface(new Ntp());
  //Tamtam->setWebsocketInterface(new EspWebsocketClient());
  Tamtam->setup();
}

void loop() {
  Tamtam->loop();
}