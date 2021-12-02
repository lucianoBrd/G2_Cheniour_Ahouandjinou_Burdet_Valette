#include <Arduino.h>
// For the screen
#include <TFT_eSPI.h>
// For wifi and mqtt
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h

void initDevice()
{
  tft.init();
  tft.begin();
  tft.setRotation(1);
  tft.setTextSize(2);
  // tft.fillScreen(TFT_BLACK);
  tft.setCursor(80, 80);
  tft.println("Hello");
  tft.setCursor(80, 120);
  tft.println("DomoTic");
  tft.setCursor(80, 170);
  tft.println("Ver1.00");
}



void setup() {
  Serial.begin(115200);
  initDevice();

  }

void loop() {
  // put your main code here, to run repeatedly:
}


