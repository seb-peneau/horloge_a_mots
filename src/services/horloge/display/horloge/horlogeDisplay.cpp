#include "horlogeDisplay.h"

int brightness = 2; // full 1

HorlogeDisplay::HorlogeDisplay() {

}

void HorlogeDisplay::setup(DebugInterface *debug) {
  displayDebug = debug;
  strip = new NeoPixelBus<NeoRgbwFeature, NeoWs2812xMethod>(PixelCount, PixelPin);
  RgbColor red = RgbColor(0, 255/brightness, 0);
  HslColor hslRed = HslColor(red);
  RgbColor green = RgbColor(255/brightness, 0, 0);
  HslColor hslGreen = HslColor(green);
  strip->Begin();
  strip->Show();
  
  strip->SetPixelColor(65,hslRed); //red
  strip->SetPixelColor(64,hslRed);
  strip->SetPixelColor(63,hslRed);
  strip->SetPixelColor(62, hslGreen);
  strip->Show();
}

void HorlogeDisplay::loop () {
}

void HorlogeDisplay::displayConfigurationMode(bool show) {
  if (show) {
    RgbColor orange = RgbColor(200, 255/brightness, 0);
    HslColor hslOrange = HslColor(orange);
    strip->SetPixelColor(62,hslOrange);
    strip->SetPixelColor(63,hslOrange);
    strip->SetPixelColor(64,hslOrange);
    strip->SetPixelColor(65,hslOrange);
    strip->Show();
  }
}

void HorlogeDisplay::clearDisplay() {
  strip->Begin();
  strip->Show();
  RgbColor black = RgbColor(0,0,0);
  HslColor hslBlack = HslColor(black);
  for (int i=0; i <= 65; i++) {
    strip->SetPixelColor(i, hslBlack);
  }
}

void HorlogeDisplay::displayWifiStatus(bool displayWifiStatus) {
  if (!displayWifiStatus) {
    RgbColor red = RgbColor(0, 255/brightness, 0);
    HslColor hslRed = HslColor(red);
    RgbColor green = RgbColor(255/brightness, 0, 0);
    HslColor hslGreen = HslColor(green);
    if (repeat < 100) {
      strip->SetPixelColor(63,hslGreen);
    } else if (repeat > 200) {
      repeat = 0;
    } else { 
      strip->SetPixelColor(63,hslRed);
    } 
    repeat = repeat + 1;
    strip->Show();
  }
}

int hours[13][3] = {
  {26,27,28}, // minuit
  {13,14,0}, // une
  {3,4,0}, // deux
  {5,6,7}, // trois  
  {8,9,10}, // quatre  
  {17,18,0}, // cinq  
  {19,20,0}, // six  
  {15,16,0}, // sept  
  {21,22,0}, // huit  
  {11,12,0}, // neuf  
  {24,25,0}, // dix  
  {33,34,0}, // onze  
  {23,24,0} // douze 
};

int minutes[][8] = {
  {45,46,0,0,0,0,0,0}, // cinq
  {43,44,0,0,0,0,0,0}, // dix
  {41,47,48,49,0,0,0,0}, // et quart
  {38,39,40,0,0,0,0,0}, // vingt
  {38,39,40,45,46,0,0,0}, // ving cinq
  {41,50,51,52,0,0,0,0}, // trente
  {35,36,37,38,39,40,45,46}, // moins ving cinq
  {35,36,37,38,39,40,0,0}, // moins ving
  {35,36,37,42,47,48,49,0}, // moins le quart
  {35,36,37,43,44,0,0,0}, // moins dix
  {35,36,37,45,46,0,0,0} // moins cinq
};

int lastHour = 0;
int lastMinute = 0;

void HorlogeDisplay::displayH(int hour) {
  displayDebug->debug("HorlogeDisplay::displayHour::"+String(hour));
  RgbColor white = RgbColor(255/brightness);
  HslColor hslWhite = HslColor(white);
  RgbColor green = RgbColor(255/brightness,0,0);
  HslColor hslGreen = HslColor(green);
  strip->SetPixelColor(0,hslWhite); // IL
  strip->SetPixelColor(1,hslWhite); // EST
  strip->SetPixelColor(2,hslWhite); // EST
  for (int i=0; i < 3; i++) {
    if (hours[hour][i] > 0) {
      strip->SetPixelColor(hours[hour][i],hslGreen);
    }
  }
  if (hour != 0 && hour != 12) {
    strip->SetPixelColor(30,hslWhite);
    strip->SetPixelColor(31,hslWhite);
    strip->SetPixelColor(32,hslWhite);
  }
  if (hour > 1 && hour != 12) {
    strip->SetPixelColor(29,hslWhite);
  }
}

void HorlogeDisplay::displayM(int minute, int remaining) {
  RgbColor white = RgbColor(255/brightness);
  HslColor hslWhite = HslColor(white);
  RgbColor orange = RgbColor(200/brightness,255/brightness,0);
  HslColor hslGreen = HslColor(orange);
  if (remaining == 0) {
    strip->SetPixelColor(53,hslWhite); // pile
    strip->SetPixelColor(54,hslWhite); // pile
  } else {
    strip->SetPixelColor(55,hslWhite); // et des bananes
    strip->SetPixelColor(56,hslWhite); // et des bananes
    strip->SetPixelColor(57,hslWhite); // et des bananes
    strip->SetPixelColor(58,hslWhite); // et des bananes
    strip->SetPixelColor(59,hslWhite); // et des bananes
    strip->SetPixelColor(60,hslWhite); // et des bananes
    strip->SetPixelColor(61,hslWhite); // et des bananes
    for (int i = 1; i <= remaining; i++) {
      strip->SetPixelColor(61+i,hslGreen);
    }
  }
  for (int i=0; i < 8; i++) {
    if (minutes[(minute/5) - 1][i] > 0) {
      strip->SetPixelColor(minutes[(minute/5) - 1][i],hslGreen);
    }
  }
}

void HorlogeDisplay::displayHour(int hour, int minute) {
  int full = (minute / 5) * 5;
  if (full > 30) {
    hour = hour+1;
    if (hour == 24) hour = 0;
  }
  if (hour > 12) {
    hour = hour - 12;
  }
  if (hour != lastHour || lastMinute != minute) {
    clearDisplay();
    int remaining = minute % 5;

    displayH(hour);
    displayM(full, remaining);
    lastMinute = minute;
    lastHour = hour;
    strip->Show();
  }
}

void HorlogeDisplay::displayOtaProgress(int progress) {
  clearDisplay();
  RgbColor blue = RgbColor(0,0,255/brightness);
  HslColor hslBlue = HslColor(blue);
  RgbColor green = RgbColor(255/brightness,0,0);
  HslColor hslGreen = HslColor(green);
  if (progress > 0) { strip->SetPixelColor(62,hslBlue); }
  if (progress > 25) { strip->SetPixelColor(63,hslBlue); }
  if (progress > 50) { strip->SetPixelColor(64,hslBlue); }
  if (progress > 75) { strip->SetPixelColor(65,hslBlue); }
  if (progress == 100) {
    strip->SetPixelColor(62,hslGreen);
    strip->SetPixelColor(63,hslGreen);
    strip->SetPixelColor(64,hslGreen);
    strip->SetPixelColor(65,hslGreen);
  }
  strip->Show();
}