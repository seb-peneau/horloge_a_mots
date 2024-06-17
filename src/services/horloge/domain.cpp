#include "domain.h"

int count = 0;

void Domain::setup() {
  debug->debug("Domain::setup");
  network->setup(debug);
  storage->setup(debug);
  webServer->setup(debug);
  display->setup(debug);
  time->setup();
  //websocket->setup(debug);

  displayWifiAnimation = false;

  // reset was triggered by user input on reset button
  // remove configuration
  if (board->isUserTriggeredReset()) {
    storage->write(SSIDSTORAGE, "");
    storage->write(PASSSTORAGE, "");
  }
  
  String ssid = storage->read(SSIDSTORAGE);
  String password = storage->read(PASSSTORAGE);
  if (ssid != "" && password != "") {
    displayWifiAnimation = true;
    counter = millis() / 1000;
    counter += 60;
    debug->debug(ssid);
    network->connect(ssid, password);
  }
}

void Domain::loop() {
  network->loop();
  webServer->loop();
  display->loop();
  display->displayWifiStatus(!displayWifiAnimation);

  // Network disconnected
  if (network->isConnected() == false) {
    // Access point is not created
    if (network->isApCreated() == false && (storage->read(SSIDSTORAGE) == "")) { // do not create AP if network is lost
      debug->debug("Domain::create Access point");
      network->disconnect();
      displayWifiAnimation = false;
      display->clearDisplay();
      bool result = network->createAccessPoint(APSSID, APPWD);
      if (result == true) { // Create Webserver
        debug->debug("Domain::create Web Server");
        display->displayConfigurationMode(true);
        webServer->startWebServer(80);
        webServer->on("/", "GET", [&, this](){
          debug->debug("Domain::on GET /");
          return handleSetupRequest ();
        });
        webServer->on("/conf", "POST", [&, this](){
          debug->debug("Domain::on POST /conf");
          return handleSaveConfiguration ();
        });
      }
    } else if (storage->read(SSIDSTORAGE) != "") {
      display->clearDisplay();
      int seconds = (millis() / 1000);
      int remaining = counter - seconds;
      float fr = ((remaining * 100) / 60) ;
      int progress = fr;
      //display->displayWifiProgress(progress);
      if (remaining < 0) {
        storage->write(SSIDSTORAGE, "");
        storage->write(PASSSTORAGE, "");
      }
    }
    if (network->isApCreated() == false && storage->read(SSIDSTORAGE) != "") {
      displayWifiAnimation = true;
    }
  } else { // Network is up and running
    if (network->isApCreated()) { // close AccessPoint and firstInstall webserver
      display->clearDisplay();
      webServer->stopWebServer();
      network->closeAccessPoint(); 
    }  
    if (displayWifiAnimation == true) {
      display->clearDisplay();
      ota->setup(debug);
      debug->debug(network->getIpAddress());
      ota->onProgress([&, this](unsigned int progress, unsigned int total){
          debug->debug("Domain::on handleOtaProgress");
          return handleOtaProgress(progress, total);
      });
      time->start();
      displayWifiAnimation = false;
    }
    ota->loop();
    time->loop();

    /*for (int i = 0; i < 13; i++) {
      display->displayHour(i, time->getMinutes());  
      delay(2000);
    }*/
    int hour = time->getHour()+1;
    if (hour == 24) {
      hour = 0;
    }
    display->displayHour(time->getHour()+1, time->getMinutes());
  }
}

/**
 * handleSetupRequest: Handle Http configuration request on main page ("/") 
 **/
void Domain::handleSetupRequest () {
  debug->debug("Domain::handleSetupRequest");
  String content = "";
  std::vector<String> networkList = network->listNetworks();
  if (networkList.size() > 0) {
    for (int i = 0; i < networkList.size(); i++) {
      content += ("<option value='" + networkList[i] + "'>" + networkList[i] + "</option>");
    }
  }
  webServer->serve(200, "text/html", SETUPSTARTCONTENT + content + SETUPENDCONTENT);
}

/**
 * handleSaveConfiguration: Handle POST request to get the configuration
 **/
void Domain::handleSaveConfiguration () {
  debug->debug("Domain::handleSaveConfiguration");
  String ssid = webServer->getBodyField("ssid");
  String pass = webServer->getBodyField("password");
  storage->write(SSIDSTORAGE, ssid);
  storage->write(PASSSTORAGE, pass);
  display->displayConfigurationMode(false);
  displayWifiAnimation = true;
  webServer->serve(200, "text/html", "<html><body>ok</body></html>");
  webServer->stopWebServer();
  network->closeAccessPoint();
  display->clearDisplay();
  counter = millis() / 1000;
  counter += 60;  
  network->connect(ssid, pass);
}

/**
 * handleConfiguration : Manage in app configuration for TAMTAM
 * 
 * */
void Domain::handleConfiguration () {
  debug->debug("Domain::handleConfiguration");
  webServer->startWebServer(80); 
  webServer->on("/", "GET", [&, this](){
    debug->debug("Domain::on GET /");
    return handleConfigurationRequest ();
  });
  webServer->on("/reset", "GET", [&, this](){
    debug->debug("Domain::on GET /reset");
    return handleSoftReset ();
  });  
}


/**
 * handleSetupRequest: Handle Http configuration request on main page ("/") 
 **/
void Domain::handleConfigurationRequest () {
  debug->debug("Domain::handleConfigurationRequest");
  webServer->serve(200, "text/html", CONFIGURATIONCONTENT);
}

/**
 * handleSoftReset: Handle configuration reset in case there is no button on the board
 */
void Domain::handleSoftReset () {
  debug->debug("Domain::handleSoftReset");
  storage->write(SSIDSTORAGE, "");
  storage->write(PASSSTORAGE, "");
  network->disconnect();
  webServer->serve(200, "text/html", "<html><body>ok</body></html>");
}

void Domain::handleOtaProgress (unsigned int progress, unsigned int total) {
  double progressP = (float) progress / (float) total;
  int progressPercent = progressP * 100;
  display->displayOtaProgress(progressPercent);
}

void Domain::setBoardInterface (BoardInterface *bI) {
  board = bI;
}

void Domain::setDebugInterface(DebugInterface* dI) {
  debug = dI;
}

void Domain::setNetworkInterface (NetworkInterface* nI) {
  network = nI;
}

void Domain::setStorageInterface (StorageInterface* sI) {
  storage = sI;
}

void Domain::setDisplayInterface (HorlogeDisplayInterface* dI) {
  display = dI;
}

void Domain::setWebServerInterface (WebServerInterface* wsI) {
  webServer = wsI;
}

void Domain::setOtaInterface (OtaInterface* oI) {
  ota = oI;
}

void Domain::setTimeInterface (TimeInterface* tI) {
  time = tI;
}