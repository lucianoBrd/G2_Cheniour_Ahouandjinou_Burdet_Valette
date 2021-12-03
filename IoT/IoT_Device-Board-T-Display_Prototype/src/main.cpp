#include <Arduino.h>
// For the screen
#include <TFT_eSPI.h>
// For wifi and mqtt
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>


#define CONNECTION_TIMEOUT 30
///_____
const char* ssid = "Redmi Note 7";
const char* password = "dallez94";


///____
TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup_Select.h
//WiFiClient espClient;
//PubSubClient client(espClient);
///____

void initScreen()
{
  // tft.init();
  tft.begin();
  tft.setRotation(0);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(40, 40);
  tft.println("Hello");
  tft.setCursor(25, 100);
  tft.println("DomoTic");
  tft.setCursor(25, 200);
  tft.println("Ver1.00");
  sleep(1);
}

void wifiConnected(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){

}

void wifiGotIP(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(5, 40);
    tft.println(WiFi.localIP());
}

void wifiDisconnected(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){
    int compteur_conn = 0;
    while(WiFi.status() != WL_CONNECTED){
       tft.fillScreen(TFT_WHITE);
       tft.setCursor(5, 40);
       tft.println(".......");       
       compteur_conn++;                         // redemarre la carte apres 30s, equivalent de 150 tentatives de connection
       if (compteur_conn >= 5*CONNECTION_TIMEOUT){
         ESP.restart();
       }
       delay(200);
       WiFi.begin(ssid, password);
    }
}


// Se connecte à un réseau WiFI
void initWifiConnection(){
    WiFi.mode(WIFI_STA); // mode station
    WiFi.onEvent(wifiConnected,SYSTEM_EVENT_STA_CONNECTED); // si l'état de la co Wifi passe à connected, appel la fct wifiConnected
    WiFi.onEvent(wifiGotIP,SYSTEM_EVENT_STA_GOT_IP);
    WiFi.onEvent(wifiDisconnected,SYSTEM_EVENT_STA_DISCONNECTED);  // si l'état de la co Wifi passe à disconnected, appel la fct wifiDisconnected, permet de se reconnecter automatiquement

    WiFi.begin(ssid, password);

    
    
}




void setup() {
  Serial.begin(115200);
  delay(1000);
  initScreen();
  initWifiConnection();

  }

void loop() {
  // put your main code here, to run repeatedly:
}


