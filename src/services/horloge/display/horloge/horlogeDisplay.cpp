#include "horlogeDisplay.h"

HorlogeDisplay::HorlogeDisplay() {

}


void HorlogeDisplay::setup(DebugInterface *debug) {
  strip = new NeoPixelBus<NeoRgbwFeature, NeoWs2812xMethod>(PixelCount, PixelPin);
  RgbColor red = RgbColor(0, 255, 0);
  HslColor hslRed = HslColor(red);
  RgbColor green = RgbColor(255, 0, 0);
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


void HorlogeDisplay::displayWifiStatus(bool displayWifiStatus) {

}