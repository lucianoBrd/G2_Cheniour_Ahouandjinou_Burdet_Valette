#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <DHT.h>

#define CONNECTION_TIMEOUT 10
#define DHTTYPE DHT11        // Definit le type de capteur utilise

#define DHTPIN 2           // pin pour la broche DATA du capteur 
#define pinLed 12         // pin pour la LED
#define pinBouton 27     // pin pour le boutton

///_____
const char* ssid = "Redmi Note 7";                 //"SFR_DDA8"; // 
const char* password = "dallez94"; //"3vsk72pjpz5fkd69umkz"; 
const char* mqttServer = "192.168.43.222";       //"broker.hivemq.com";//IPAddress my_IPAddress(192,168,43,222);
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* idCapteur = "Temp - Hum";

int etat = -1;               //  variable representant l'etat du capteur
int pwmChannel = 0;         // channel de O-15 disponibles
double pwmFreq = 0;         // frequence pwm
int pwmResolution = 16;     //8-16 bits possibles

float mesure_temp = 0;
float mesure_hum = 0;

char* etatWifi = "";
char* etatMqtt = "";

///____ Appel classes
TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup_Select.h
WiFiClient espClient;
PubSubClient mqttClient(espClient);
DHT dht(DHTPIN, DHTTYPE);  // Declare un objet de type DHT// Il faut passer en parametre du constructeur // de l'objet la broche et le type de capteur

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

void GestionIHM(float temp, float hum){
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(5, 20);
  tft.println(idCapteur);           //mqttClient.state());
  tft.setCursor(5, 50);
  tft.println("__________");


  tft.setCursor(5, 100);
  tft.println("Tem : ");
  tft.setCursor(70, 100);
  tft.println(mesure_temp);

  tft.setCursor(5, 140);
  tft.println("Hum : ");
  tft.setCursor(70, 140);
  tft.println(mesure_hum);

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
 // while (WiFi.status() != WL_CONNECTED){}
  
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) {
      
    if (mqttClient.connect(idCapteur, mqttUser, mqttPassword )) {
      etatMqtt = "OK";  
      } else {  
      etatMqtt = "KO";
      delay(2000);
      }
    mqttClient.setCallback(mqttCallback);
    GestionIHM(0,0);
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
  GestionIHM(0,0);
  initWifiConnection();

// Config pins
  pinMode(12, OUTPUT);
  pinMode(pinBouton,INPUT_PULLDOWN);
    
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);  // Configuration du canal 0 avec la fréquence et la résolution choisie
  ledcAttachPin(pinLed, pwmChannel);        // assigne le canal PWM au pin 
  dht.begin();                              // demare le capteur dht11 
  attachInterrupt(pinBouton, gestionBouton, FALLING); // attache une interruption sur le bouton, front descendant, appel gestionBouton
  }

void loop() {
  
  if (!mqttClient.connected()){etat = 0;etatMqtt = "KO";}       // si le client mqtt est déconnecté
  else{etat = 1;etatMqtt = "OK";}
  if (WiFi.status() != WL_CONNECTED){etatWifi = "KO";}
  else{etatWifi = "OK";}

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
      GestionIHM(0,0);
      initMqtt();                                 // tentative de (re)connection du client mqtt au brooker

    break;

  case 1:
    mesure_temp = dht.readTemperature();
    mesure_hum = dht.readHumidity();
    
    mqttClient.publish("esp/test", "Hello from ESP32");
    if (mesure_temp != NULL){
    mqtt_publish_float("home/living_room/sensor_temperature/temperature",mesure_temp);       // publication de la temperature sur le topic
    }
    if (mesure_hum != NULL){
    mqtt_publish_float("home/living_room/sensor_humidity/humidity",mesure_hum);
    }

    GestionIHM(mesure_temp,mesure_hum);

    if (pwmFreq!=0.1){
      pwmFreq =0.1;
      ledcDetachPin(pinLed);
      ledcSetup(pwmChannel, pwmFreq, pwmResolution);
      ledcAttachPin(pinLed, pwmChannel);
      ledcWrite(pwmChannel, 10);
    }  
    
    delay(5000);
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
