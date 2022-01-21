#include <Arduino.h>
// For the screen
#include <TFT_eSPI.h>
// For wifi and mqtt
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>


#define CONNECTION_TIMEOUT 30
#define pinoutR 26
#define pinoutJ 25
#define pinoutV 27
///_____
const char* ssid = "Domotique";
const char* password = "Domotique";
const char* mqttServer = "192.168.223.222";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* idCapteur = "Lumière";
int mode = 0;
char* Cmode = "OFF";
char* etatWifi = "";
char* etatMqtt = "";


///____ Appel classes
TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup_Select.h
WiFiClient espClient;
PubSubClient mqttClient(espClient);
///____

//init pins
void initPins(){
  pinMode(pinoutJ,OUTPUT);
  pinMode(pinoutR,OUTPUT);
  pinMode(pinoutV,OUTPUT);

}
void mode_etein(){
  digitalWrite(pinoutJ,LOW);
  digitalWrite(pinoutR,LOW);
  digitalWrite(pinoutV,LOW);
}
void mode_all_allumer(){
  digitalWrite(pinoutJ,HIGH);
  digitalWrite(pinoutR,HIGH);
  digitalWrite(pinoutV,HIGH);
}
void mode_rouge(){
  digitalWrite(pinoutJ,LOW);
  digitalWrite(pinoutR,HIGH);
  digitalWrite(pinoutV,LOW);
}
void mode_vert(){
  digitalWrite(pinoutJ,LOW);
  digitalWrite(pinoutR,LOW);
  digitalWrite(pinoutV,HIGH);
}
void mode_jaune(){
  digitalWrite(pinoutJ,HIGH);
  digitalWrite(pinoutR,LOW);
  digitalWrite(pinoutV,LOW);
}
void mode_fete(){
  digitalWrite(pinoutJ,LOW);
  digitalWrite(pinoutR,LOW);
  digitalWrite(pinoutV,LOW);
  digitalWrite(pinoutJ,HIGH);
  delay(100);
  digitalWrite(pinoutJ,LOW);
  delay(100);
  digitalWrite(pinoutR,HIGH);
  delay(100);
  digitalWrite(pinoutR,LOW);
  delay(100);
  digitalWrite(pinoutV,HIGH);
  delay(100);
  digitalWrite(pinoutV,LOW);
}
void mode_alarm(){
  digitalWrite(pinoutJ,LOW);
  digitalWrite(pinoutR,LOW);
  digitalWrite(pinoutV,LOW);
  digitalWrite(pinoutR,HIGH);
  delay(100);
  digitalWrite(pinoutR,LOW);
  delay(100);
}
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
  tft.setCursor(25, 20);
  tft.println(idCapteur);           //mqttClient.state());
  tft.setCursor(5, 50);
  tft.println("__________");


  tft.setCursor(5, 100);
  tft.println("Mode: ");
  tft.setCursor(70, 100);
  tft.println(Cmode);

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
     mode = 0;
     Cmode = "OFF";
     mqttClient.publish("home/living_room/actuator_lum/state/acquittement", "OFF RECEIVED");
     GestionIHM();
     }
   else if ((char)payload[0] == '1'){
     mode = 1;
     Cmode = "ON";
     mqttClient.publish("home/living_room/actuator_lum/state/acquittement", "ON RECEIVED");
     GestionIHM();
     }
     else if ((char)payload[0] == '2'){
     mode = 2;
     Cmode = "ROUGE";
     mqttClient.publish("home/living_room/actuator_lum/state/acquittement", "ROUGE RECEIVED");
     GestionIHM();
     }
     else if ((char)payload[0] == '3'){
     mode = 3;
     Cmode = "VERT";
     mqttClient.publish("home/living_room/actuator_lum/state/acquittement", "VERT RECEIVED");
     GestionIHM();
     }
     else if ((char)payload[0] == '4'){
     mode = 4;
     Cmode = "JAUNE";
     mqttClient.publish("home/living_room/actuator_lum/state/acquittement", "JAUNE RECEIVED");
     GestionIHM();
     }
     else if ((char)payload[0] == '5'){
     mode = 5;
     Cmode = "FETE";
     mqttClient.publish("home/living_room/actuator_lum/state/acquittement", "FETE RECEIVED");
     GestionIHM();
     }
     else if ((char)payload[0] == '6'){
     mode = 6;
     Cmode = "ALARME";
     mqttClient.publish("home/living_room/actuator_lum/state/acquittement", "ALARME RECEIVED");
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
    bool a = mqttClient.subscribe("home/living_room/actuator_lum/state",1);//subscribe to topic
    if (a == true){Serial.println("Subscribed to topic");}
    delay(200);
    }
  
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  initScreen();
  initPins();
  initWifiConnection();
  initMqtt();
  GestionIHM();
  




  }

void loop() {
  
  mqttClient.loop();
  
  
  //if (!mqttClient.connected()){
  //     initMqtt();
  //}
  switch(mode)
  {
    case 0:
      mode_etein();
      break;
    case 1:
      mode_all_allumer();
      break;
    case 2:
      mode_rouge();
      break;
    case 3:
      mode_vert();
      break;
    case 4:
      mode_jaune();
      break;
    case 5:
      mode_fete();
      break;
    case 6:
      mode_alarm();
      break;
    default:
      break;



  }


  //mqttClient.publish("esp/send", "La rue");
  
  //delay(2000);

}


/// note 
/* il faudra peut-être changer l'adresse mac des cartes
http://www.hivemq.com/demos/websocket-client/
https://www.hivemq.com/public-mqtt-broker/
*/
