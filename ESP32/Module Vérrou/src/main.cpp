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
const char* ssid = "Domotique";
const char* password = "Domotique";
const char* mqttServer = "192.168.12.222";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* idCapteur = "Verrou";
char* Etat_lock = "CLOSE";
char* etatWifi = "";
char* etatMqtt = "";
//Config PWM
int pwmChannel = 0;  // channel de O-15
int pwmFreq = 50; 
int pwmResolution = 8;  // résolution de la largeur d’impulsion entre 1 et 16 bits
int pwmPin = 12;
void initPWM(){
  // Config pins
  // pinMode(2, OUTPUT);
  // pinMode(12, OUTPUT);
  
  // digitalWrite(2, HIGH);
  // Configuration du canal 0 avec la fréquence et la résolution choisie
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);

   // Assigne le canal PWM au pin 36
  ledcAttachPin(pwmPin, pwmChannel);
  ledcWrite(pwmChannel, 17);
}
void motorGestion(int state){

  if (state == 1){
  // Créer la tension en sortie choisi
    ledcWrite(pwmChannel, 28); //OPEN
  }
  else if (state == 0){
    ledcWrite(pwmChannel, 17); //CLOSE
  }

}

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
void GestionIHM(){
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(35, 20);
  tft.println(idCapteur);           //mqttClient.state());
  tft.setCursor(5, 50);
  tft.println("__________");


  tft.setCursor(5, 100);
  tft.println("Lock: ");
  tft.setCursor(70, 100);
  tft.println(Etat_lock);

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
}

void wifiDisconnected(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){
    int compteur_conn = 0;
    etatWifi = "KO";
    GestionIHM();
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED){     
       compteur_conn++;                         // redemarre la carte apres 30s, equivalent de 150 tentatives de connection
       if (compteur_conn >= 5*CONNECTION_TIMEOUT){
         ESP.restart();
       }
      WiFi.begin(ssid, password); 
       
    }
}

// Se connecte à un réseau WiFI
// Gestion WiFi par les event de la co
void initWifiConnection(){
    WiFi.mode(WIFI_STA); // mode station
    WiFi.onEvent(wifiConnected,SYSTEM_EVENT_STA_CONNECTED); // si l'état de la co Wifi passe à connected, appel la fct wifiConnected
    WiFi.onEvent(wifiGotIP,SYSTEM_EVENT_STA_GOT_IP);        // // si la carte recoit une @IP appel la fct wifiGotIP
    WiFi.onEvent(wifiDisconnected,SYSTEM_EVENT_STA_DISCONNECTED);  // si l'état de la co Wifi passe à disconnected, appel la fct wifiDisconnected, permet de se reconnecter automatiquement

    WiFi.begin(ssid, password);
}

void mqttCallback(char* topic, uint8_t* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
  //Serial.print((char)payload[0]);
   if( (char)payload[0] == '0'){
     Etat_lock = "CLOSE";
     motorGestion(0);
     mqttClient.publish("home/living_room/actuator_entry_door/state/acquittement", "OFF RECEIVED");
     GestionIHM();
     }
   else if ((char)payload[0] == '1'){
     Etat_lock = "OPEN";
     motorGestion(1);
     mqttClient.publish("home/living_room/actuator_entry_door/state/acquittement", "ON RECEIVED");
     GestionIHM();
     }

}

void initMqtt(){
  while (WiFi.status() != WL_CONNECTED){}
  
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) {
    //Serial.println("Connecting to MQTT...");
  
    if (mqttClient.connect(idCapteur, mqttUser, mqttPassword )) {
      Serial.println("connected");
      etatMqtt = "OK";
      GestionIHM();
      } else {
  
      Serial.print("failed with state ");
      etatMqtt = "KO";
      GestionIHM();
      delay(2000);
      }
    mqttClient.setCallback(mqttCallback);
    bool a = mqttClient.subscribe("home/living_room/actuator_entry_door/state",1);//subscribe to topic
    if (a == true){Serial.println("Subscribed to topic");}
    delay(200);
    }
  
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  initScreen();
  initWifiConnection();
  initMqtt();
  initPWM();
  GestionIHM();
  




  }

void loop() {
  
  mqttClient.loop();
  
  
   if (!mqttClient.connected()){
       initMqtt();
  }
  //mqttClient.publish("esp/send", "La rue");
  
  //delay(2000);

}


/// note 
/* il faudra peut-être changer l'adresse mac des cartes
http://www.hivemq.com/demos/websocket-client/
https://www.hivemq.com/public-mqtt-broker/
*/
