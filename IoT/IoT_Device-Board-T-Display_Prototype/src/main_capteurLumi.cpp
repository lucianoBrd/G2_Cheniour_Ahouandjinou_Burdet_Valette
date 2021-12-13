#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>


#define CONNECTION_TIMEOUT 10
#define pinBouton 27     // pin pour le boutton
#define pinLed 12         // pin pour la LED
#define pinPhotoresistance 32

///_____
const char* ssid = "SFR_DDA8";                   //"Redmi Note 7"; // 
const char* password = "3vsk72pjpz5fkd69umkz";  // dallez94
const char* mqttServer = "192.168.1.89";       //"broker.hivemq.com";//IPAddress my_IPAddress(192,168,43,222);
const int mqttPort = 1883;
const char* mqttUser = "module_lumi01";
const char* mqttPassword = "";

int etat = -1;
int pwmChannel = 0;         // channel de O-15 disponibles
double pwmFreq = 0;         // frequence pwm
int pwmResolution = 16;     //8-16 bits possibles
int intensiteLum = -1;

///____ Appel classes
TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup_Select.h
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

void wifiConnected(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){

}

void wifiGotIP(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(5, 40);
    tft.println(WiFi.localIP());
    delay(1000);
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
 // while (WiFi.status() != WL_CONNECTED){}
  
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) {
      
    if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword )) {
      tft.fillScreen(TFT_WHITE);
      tft.setCursor(5, 90);
      tft.println("Mqtt:OK");
  
      } else {
  
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      tft.fillScreen(TFT_WHITE);
      tft.setCursor(5, 90);
      tft.println("Mqtt:KO");//mqttClient.state());
      delay(2000);
      }
    mqttClient.setCallback(mqttCallback);
    
    }
    mqttClient.loop(); 
  
}

void gestionBouton(){
  // la pin 4 correspond à la backlight
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

  if (!mqttClient.connected()){etat = 0;}       // si le client mqtt est déconnecté
  else{etat = 1;}

  switch (etat)
  {
    case 0:               // si état=0, client mqtt est déconnecté
      
      tft.fillScreen(TFT_WHITE);
      tft.setCursor(50, 150);
      tft.println(etat);

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
      initMqtt();                                 // tentative de (re)connection du client mqtt au brooker
    break;

  case 1:
  
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(5, 90);
    tft.println();           //mqttClient.state());
    tft.setCursor(50, 150);
    tft.println(etat);

    mqttClient.publish("esp/test", "Hello from ESP32");
  //  mqtt_publish_float("esp/hum",mesure_hum);
    
    if (pwmFreq!=0.1){
      pwmFreq =0.1;
      ledcDetachPin(pinLed);
      ledcSetup(pwmChannel, pwmFreq, pwmResolution);
      ledcAttachPin(pinLed, pwmChannel);
      ledcWrite(pwmChannel, 50);
    }  
    
     delay(2000);
     intensiteLum = analogRead(pinPhotoresistance);
     tft.fillScreen(TFT_WHITE);
     tft.setCursor(5, 90);
     tft.println(intensiteLum);    
     delay(2000);
    break;
      
  default:  
    break;
  }
}


/// note 
/* il faudra peut-être changer l'adresse mac des cartes

Def pins:

#define TFT_MOSI            19
#define TFT_SCLK            18
#define TFT_CS              5
#define TFT_DC              16
#define TFT_RST             23
#define TFT_BL              4 

PWM : all pins sauf les pins GPIO36, GPIO39, GPIO34, GPIO35
12 --> led

GPIO : 
2 --> capteur
12 --> bouton


*/
