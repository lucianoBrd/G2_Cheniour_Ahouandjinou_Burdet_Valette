#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>


#define CONNECTION_TIMEOUT 10
#define pinBouton 27                        // pin pour le boutton
#define pinLed 12                           // pin pour la LED
#define pinPhotoresistance 32               // pin pour la photoresistance

///_____
const char *ssid = "Domotique";                  // identifiant réseau
const char *password = "Domotique";              // mot de passe réseau
const char *mqttServer = "192.168.43.222";       // addresse IP du brooker Mqtt
const int mqttPort = 1883;
const char* mqttUser = "module_lumi01";
const char* mqttPassword = "";

char* etatWifi = "";
char* etatMqtt = "";

char* idCapteur = "Mod Lumi";

int etat = -1;              // état du système
int pwmChannel = 0;         // channel de O-15 disponibles
double pwmFreq = 0;         // frequence pwm
int pwmResolution = 16;     //8-16 bits possibles
int intensiteLum = -1;

///____ Appel classes
TFT_eSPI tft = TFT_eSPI();      // appel library, pins définie dans User_Setup_Select.h
WiFiClient espClient;
PubSubClient mqttClient(espClient);

///____
//Demarre l'ecran 
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

void GestionIHM(int lum){
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(15, 20);
  tft.println(idCapteur);           //mqttClient.state());
  tft.setCursor(5, 50);
  tft.println("__________");

  tft.setCursor(5, 100);
  tft.println("Lum : ");
  tft.setCursor(70, 100);
  tft.println(lum);
  tft.setCursor(100, 100);
  tft.println("%");

  tft.setCursor(5, 190);
  tft.println("WiFi : ");
  tft.setCursor(100, 190);
  tft.println(etatWifi);

  tft.setCursor(5, 220);
  tft.println("Mqtt : ");
  tft.setCursor(100, 220);
  tft.println(etatMqtt);

}

void wifiConnected(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){
  etatWifi = "OK";
}

void wifiGotIP(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(5, 40);
    tft.println(WiFi.localIP());
    delay(1000);
    etatWifi = "OK";
}

void wifiDisconnected(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){
    etatWifi = "KO";
    int compteur_conn = 0;
    while(WiFi.status() != WL_CONNECTED){  
       compteur_conn++;                         // redemarre la carte apres 30s, equivalent de 150 tentatives de connection
       if (compteur_conn >= 5*CONNECTION_TIMEOUT){
         ESP.restart();
       }    
       WiFi.begin(ssid, password); delay(100);
    }
}

// Se connecte à un réseau WiFI
// Gestion WiFi par les event de la connection
void initWifiConnection(){
    WiFi.mode(WIFI_STA); // mode station
    WiFi.onEvent(wifiConnected,SYSTEM_EVENT_STA_CONNECTED); // si l'état de la co Wifi passe à connected, appel la fct wifiConnected
    WiFi.onEvent(wifiGotIP,SYSTEM_EVENT_STA_GOT_IP);        // // si la carte recoit une @IP appel la fct wifiGotIP
    WiFi.onEvent(wifiDisconnected,SYSTEM_EVENT_STA_DISCONNECTED);  // si l'état de la co Wifi passe à disconnected, appel la fct wifiDisconnected, permet de se reconnecter automatiquement

    WiFi.begin(ssid, password);
}

void mqttCallback(char* valeur, uint8_t* var, unsigned int nb){

}

void initMqtt(){

  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) {
      
    if (mqttClient.connect(idCapteur, mqttUser, mqttPassword )) {
      etatMqtt = "OK";  
      } else {  
      etatMqtt = "KO";
      delay(2000);
      }
    mqttClient.setCallback(mqttCallback);
    GestionIHM(0);
    }
    mqttClient.loop();
}

void gestionBouton(){
  // la pin 4 correspond à la backlight de l'écran TFT
  if (digitalRead(4)!=LOW){digitalWrite(4,LOW);}
  else{digitalWrite(4,HIGH);} 
}

//Fonction pour publier un float sur un topic 
void mqtt_publish_float(String topic, float t){
  char top[topic.length()+1];
  topic.toCharArray(top,topic.length()+1);
  char t_char[50];
  String t_str = String(t);
  t_str.toCharArray(t_char, t_str.length() + 1);
  mqttClient.publish(top,t_char);
}

void setup() {
  // Init peripheriques
  Serial.begin(115200);
  delay(1000);
  initScreen();
  initWifiConnection();

// Config pins
  pinMode(pinPhotoresistance,INPUT);
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);  // Configuration du canal 0 avec la fréquence et la résolution choisie
  ledcAttachPin(pinLed, pwmChannel);        // assigne le canal PWM au pin 
  pinMode(pinBouton,INPUT_PULLDOWN);
  attachInterrupt(pinBouton, gestionBouton, FALLING); // attache une interruption sur le bouton, front descendant, appel gestionBouton
//   
}

void loop() {
  mqttClient.loop();
  if (!mqttClient.connected()){etat = 0;etatMqtt = "KO";}       // si le client mqtt est déconnecté
  else{etat = 1;etatMqtt = "OK";}
  if (WiFi.status() != WL_CONNECTED){etatWifi = "KO";}
  else{etatWifi = "OK";}

  intensiteLum = analogRead(pinPhotoresistance);          // lecture de la valeur de la photoresistance
  switch (etat)
  {
    case 0:               // si état=0, client mqtt est déconnecté
      
      if(pwmFreq!=5){                   // Gestion led, entre en mode "Tentative de (re)connection", cligonte rapidement
        pwmFreq = 5;
        ledcDetachPin(pinLed);
        ledcSetup(pwmChannel, pwmFreq, pwmResolution);
        ledcAttachPin(pinLed, pwmChannel);
        ledcWrite(pwmChannel, 50);
      }

      if (WiFi.status() != WL_CONNECTED){         // Contrôle status connection Wifi, reconnection si déconnecté
        WiFi.reconnect();
      }
      initMqtt();  
      GestionIHM(0);
                                     // tentative de (re)connection du client mqtt au brooker
    break;

  case 1:
    intensiteLum = intensiteLum * 100 / 500;     // formattage valeur, en pourcentage
    GestionIHM(intensiteLum);                     // affiche valeur sur l'écran
    if (intensiteLum > 100){intensiteLum = 100;}
    if (intensiteLum)
    {
      mqtt_publish_float("home/living_room/sensor_luminosity/luminosity", intensiteLum);    // publication de la valeur sur le topic correspondant
   } 
    
    if (pwmFreq!=0.1){                                                  // réglage du signal PWM en fonctionne de l'état du système 
      pwmFreq =0.1;
      ledcDetachPin(pinLed);
      ledcSetup(pwmChannel, pwmFreq, pwmResolution);
      ledcAttachPin(pinLed, pwmChannel);
      ledcWrite(pwmChannel, 50);
    }   
    delay(2000);
  break;
      
  default:  
    break;
  }
}


