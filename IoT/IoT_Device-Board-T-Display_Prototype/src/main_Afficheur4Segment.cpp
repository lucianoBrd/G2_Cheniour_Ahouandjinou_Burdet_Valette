#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>

#define CONNECTION_TIMEOUT 10
// Pins utilisés sur la carte
#define pinDigitA0 33   // selection digit ( A0A1 : 00 --> Digit1, 10 --> Digit2, 01 --> Digit3, 11 --> Digit4 ) 
#define pinDigitA1 32

#define pinSegmentA 27     // segment à allumer, actif à l'état haut
#define pinSegmentB 15  //36
#define pinSegmentC 2   //37
#define pinSegmentD 12
#define pinSegmentE 13
#define pinSegmentF 26// 26
#define pinSegmentG 25 

#define pinBtnIncrementerChiffre 37               // bouton permettant d'incrementer la valeur d'un digit (7 segment)
#define pinBtnDigitSuivant 38                    // bouton permettant de passer au digit suivant
#define pinBtnValiderMdp 39                      // bouton permettant de valider le mdp
///_____
const char* ssid = "Domotique";               // identifiant réseau           //"SFR_DDA8"; // 
const char* password = "Domotique";               // mot de passe réseau        
const char* mqttServer = "192.168.12.222";       // addresse IP du brooker Mqtt
const int mqttPort = 1883;                       // port de connection mqtt
const char* mqttUser = "";
const char* mqttPassword = "";
const char* idCapteur = "Digicode";              // identifiant du module 
const int mdpPorte = 2468;

int etat = -1;                                   // variable representant l'etat du capteur
int digitActuel = 1;                             // on demarre positionné sur le digit 1
int valeur7segment[] = {1,2,3,4};

char* etatWifi = "";                             // paramètre permettant de récuperer l'état de connection Wifi
char* etatMqtt = "";                             // paramètre permettant de récuperer l'état de connection MQTT

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
  tft.setCursor(15, 20);
  tft.println(idCapteur);           //mqttClient.state());
  tft.setCursor(5, 50);
  tft.println("__________");


  tft.setCursor(5, 100);
  tft.println("Tem : ");
  tft.setCursor(70, 100);
  tft.println("");

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
    GestionIHM();
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

void selectDigit (int digit){
 switch (digit)
 {
  case 1:
        digitalWrite(pinDigitA0,LOW);
        digitalWrite(pinDigitA1,LOW);

         break;
  case 2:
        digitalWrite(pinDigitA0,HIGH);
        digitalWrite(pinDigitA1,LOW);
         break;
  case 3:
        digitalWrite(pinDigitA0,LOW);
        digitalWrite(pinDigitA1,HIGH);
         break;
  case 4:
        digitalWrite(pinDigitA0,HIGH);
        digitalWrite(pinDigitA1,HIGH);
         break;
 
 default:
         break;
 }

}

void chiffre(int chiffre){
        switch (chiffre)
        {

        case -1:
                digitalWrite(pinSegmentA, LOW);
                digitalWrite(pinSegmentB, LOW);
                digitalWrite(pinSegmentC, LOW);
                digitalWrite(pinSegmentD, LOW);
                digitalWrite(pinSegmentE, LOW);
                digitalWrite(pinSegmentF, LOW);
                digitalWrite(pinSegmentG, LOW);
                break;

        case 0:
                digitalWrite(pinSegmentA,HIGH);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,HIGH);
                digitalWrite(pinSegmentE,HIGH);
                digitalWrite(pinSegmentF,HIGH);
                digitalWrite(pinSegmentG,LOW);
                break;
                
        case 1:
                digitalWrite(pinSegmentA,LOW);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,LOW);
                digitalWrite(pinSegmentE,LOW);
                digitalWrite(pinSegmentF,LOW);
                digitalWrite(pinSegmentG,LOW);
                break;
        case 2:
                digitalWrite(pinSegmentA,HIGH);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,LOW);
                digitalWrite(pinSegmentD,HIGH);
                digitalWrite(pinSegmentE,HIGH);
                digitalWrite(pinSegmentF,LOW);
                digitalWrite(pinSegmentG,HIGH);
                break;
        case 3:
                digitalWrite(pinSegmentA,HIGH);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,HIGH);
                digitalWrite(pinSegmentE,LOW);
                digitalWrite(pinSegmentF,LOW);
                digitalWrite(pinSegmentG,HIGH);
                break;
        case 4:
                digitalWrite(pinSegmentA,LOW);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,LOW);
                digitalWrite(pinSegmentE,LOW);
                digitalWrite(pinSegmentF,HIGH);
                digitalWrite(pinSegmentG,HIGH);
                break;
        case 5:
                digitalWrite(pinSegmentA,HIGH);
                digitalWrite(pinSegmentB,LOW);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,HIGH);
                digitalWrite(pinSegmentE,LOW);
                digitalWrite(pinSegmentF,HIGH);
                digitalWrite(pinSegmentG,HIGH);
                break;
        case 6:
                digitalWrite(pinSegmentA,HIGH);
                digitalWrite(pinSegmentB,LOW);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,HIGH);
                digitalWrite(pinSegmentE,HIGH);
                digitalWrite(pinSegmentF,HIGH);
                digitalWrite(pinSegmentG,HIGH);
                break;
        case 7:
                digitalWrite(pinSegmentA,HIGH);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,LOW);
                digitalWrite(pinSegmentE,LOW);
                digitalWrite(pinSegmentF,LOW);
                digitalWrite(pinSegmentG,LOW);
                break;
        case 8:
                digitalWrite(pinSegmentA,HIGH);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,HIGH);
                digitalWrite(pinSegmentE,HIGH);
                digitalWrite(pinSegmentF,HIGH);
                digitalWrite(pinSegmentG,HIGH);
                break;
        case 9:
                digitalWrite(pinSegmentA,HIGH);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,HIGH);
                digitalWrite(pinSegmentE,LOW);
                digitalWrite(pinSegmentF,HIGH);
                digitalWrite(pinSegmentG,HIGH);
                break;
                
        default:
                digitalWrite(pinSegmentA,LOW);
                digitalWrite(pinSegmentB,LOW);
                digitalWrite(pinSegmentC,LOW);
                digitalWrite(pinSegmentD,LOW);
                digitalWrite(pinSegmentE,LOW);
                digitalWrite(pinSegmentF,LOW);
                digitalWrite(pinSegmentG,HIGH);
                break;
        }
}

void verificationMdp(){

        if ((1000*valeur7segment[0]+100*valeur7segment[1]+10*valeur7segment[2]+valeur7segment[3])==mdpPorte){
              //  mqttClient.publish("home/living_room/actuator_entry_door/state", "1");
                valeur7segment[0] = 0;
                valeur7segment[1] = 0;
                valeur7segment[2] = 0;
                valeur7segment[3] = 0;
                Serial.println("bon");
        }
      
}

void incrementerChiffre(){
 switch (digitActuel)
 {

 case 1:
        valeur7segment[0] = valeur7segment[0] + 1;
        if (valeur7segment[0] > 9 ) {valeur7segment[0] = 0;}
        break;
 case 2:
        valeur7segment[1] = valeur7segment[1] + 1;
        if (valeur7segment[1] > 9 ) {valeur7segment[1] = 0;}
        break;
 case 3:
        valeur7segment[2] = valeur7segment[2] + 1;
        if (valeur7segment[2] > 9 ) {valeur7segment[2] = 0;}
        break;
 case 4:
        valeur7segment[3] = valeur7segment[3] + 1;
        if (valeur7segment[3] > 9 ) {valeur7segment[3] = 0;}
        break;
 
 default:
         break;
 }


}
void nextDigit(){
        digitActuel = digitActuel + 1;
        if (digitActuel > 4 ){
                digitActuel = 1;
        } 
}


void init7Segment(){
// Config pins 7Segement
 pinMode(pinDigitA0, OUTPUT);
 pinMode(pinDigitA1, OUTPUT);
//----------------- Pins segments
 pinMode(pinSegmentA, OUTPUT);
 pinMode(pinSegmentB, OUTPUT);
 pinMode(pinSegmentC, OUTPUT);
 pinMode(pinSegmentD, OUTPUT);
 pinMode(pinSegmentE, OUTPUT);
 pinMode(pinSegmentF, OUTPUT);
 pinMode(pinSegmentG, OUTPUT);
 //------- Pins bouttons
 pinMode(pinBtnDigitSuivant,INPUT_PULLUP);
 pinMode(pinBtnIncrementerChiffre,INPUT_PULLUP);
 pinMode(pinBtnValiderMdp,INPUT_PULLUP);
 attachInterrupt(pinBtnIncrementerChiffre,incrementerChiffre,FALLING);
 attachInterrupt(pinBtnDigitSuivant,nextDigit,FALLING);
 attachInterrupt(pinBtnValiderMdp,verificationMdp,FALLING);

}


void setup() {
  // Init peripheriques
  Serial.begin(115200);
  delay(1000);
  initScreen();
  GestionIHM();
  initWifiConnection();
  init7Segment();

 }

void loop() {
  
  if (!mqttClient.connected()){etat = 0;etatMqtt = "KO";}       // si le client mqtt est déconnecté
  else{etat = 1;etatMqtt = "OK";}
  if (WiFi.status() != WL_CONNECTED){etatWifi = "KO";}
  else{etatWifi = "OK";}

  switch (etat)
  {
    case 0:               // si état=0, client mqtt est déconnecté
    
      if (WiFi.status() != WL_CONNECTED){         // Contrôle status connection Wifi, reconnection si déconnecté
        WiFi.reconnect();
      }
      GestionIHM();
      initMqtt();                                 // tentative de (re)connection du client mqtt au brooker

    break;

  case 1:
   // mqttClient.publish("esp/test", idCapteur);
   // GestionIHM();
   chiffre(-1);
   digitalWrite(pinDigitA1, LOW);
   digitalWrite(pinDigitA0, LOW);
   delayMicroseconds(1);
   chiffre(valeur7segment[0]);
   delay(6);

   chiffre(-1);
   digitalWrite(pinDigitA0, HIGH);
   digitalWrite(pinDigitA1, LOW);
   delayMicroseconds(1);
   chiffre(valeur7segment[1]);
   delay(6);
   

   chiffre(-1);
   digitalWrite(pinDigitA1, HIGH);
   digitalWrite(pinDigitA0, LOW);
   delayMicroseconds(1);
   chiffre(valeur7segment[2]);
   delay(6);

   chiffre(-1);
   digitalWrite(pinDigitA0, HIGH);
   digitalWrite(pinDigitA1, HIGH);
   delayMicroseconds(1);
   chiffre(valeur7segment[3]);
   delay(6);

//    tft.fillScreen(TFT_WHITE);
//    tft.setCursor(40, 40);
//    tft.println(valeur7segment[0]);
//    tft.setCursor(50, 40);
//    tft.println(valeur7segment[1]);
//    tft.setCursor(60, 40);
//    tft.println(valeur7segment[2]);
//    tft.setCursor(70, 40);
//    tft.println(valeur7segment[3]);

   // delay(5000);
   break;
      
  default:  
    break;
  }
}

/*

Pins utilisés par l'écran

#define TFT_MOSI            19
#define TFT_SCLK            18
#define TFT_CS              5
#define TFT_DC              16
#define TFT_RST             23
#define TFT_BL              4 

PWM : all pins sauf les pins GPIO36, GPIO39, GPIO34, GPIO35
12 --> led

*/
