#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <BLEDevice.h>


#define CONNECTION_TIMEOUT 10
// Pins utilisés sur la carte
#define pinDigitA0 33                           // selection digit ( A0A1 : 00 --> Digit1, 10 --> Digit2, 01 --> Digit3, 11 --> Digit4 ) 
#define pinDigitA1 32

#define pinSegmentA 27                          // segments du 4*7Segments, actif à l'état haut
#define pinSegmentB 15  
#define pinSegmentC 2   
#define pinSegmentD 12
#define pinSegmentE 13
#define pinSegmentF 26
#define pinSegmentG 25 

#define pinBtnIncrementerChiffre 37              // bouton permettant d'incrementer la valeur d'un digit (7 segment)
#define pinBtnDigitSuivant 38                    // bouton permettant de passer au digit suivant
#define pinBtnValiderMdp 17                      // bouton permettant de valider le mdp

#define pinLedVert 22
#define pinLedRouge 21

///_____
const char *ssid = "Domotique";                  // identifiant réseau
const char *password = "Domotique";              // mot de passe réseau
const char* mqttServer = "192.168.43.222";       // addresse IP du brooker Mqtt
const int mqttPort = 1883;                       // port de connection mqtt
const char* mqttUser = "";
const char* mqttPassword = "";
const char* idCapteur = "Digicode";              // identifiant du module 
int mdpPorte[] = {2,3,4,5};                      // mot de passe de la porte

int etat = -1;                                   // variable representant l'etat du système : (-1) --> état initial, (0) --> MQTT KO ,(1) --> MQTT OK
volatile int digitActuel = 1;                    // variable correspondant au digit à allumer, on demarre positionné sur le digit 1
volatile int valeur7segment[] = {1,2,3,4};       // valeur affiché sur le 7Segment, l'affichage sur le 4*7Segments se fait digit par digit. On selectionne le premier digit et on affiche le premier chiffre de la liste, ainsi de suite 
int codeValide = 0;                              // passe à 1 si le code entré est bon, -1 si pas bon
int cptTentativeRecoMqtt = 0;                                  

char *etatWifi = "";                             // paramètre permettant de récuperer l'état de connection Wifi
char* etatMqtt = "";                             // paramètre permettant de récuperer l'état de connection MQTT
// BLE
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLEAdvertisedDevice *myDevice;
static BLERemoteCharacteristic *pRemoteCharacteristic;

///____ Appel classes
TFT_eSPI tft = TFT_eSPI();                       // appel library, pins définie dans User_Setup_Select.h
WiFiClient espClient;
PubSubClient mqttClient(espClient);

BLECharacteristic *pCharacteristic;
BLEScan *pBLEScan = BLEDevice::getScan();
///____

// Demarre l'ecran
void initScreen()
{
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
//Gestion de l'IHM, permet d'afficher une donnée
void GestionIHM(char* info){
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(15, 20);
  tft.println(idCapteur);           //mqttClient.state());
  tft.setCursor(5, 50);
  tft.println("__________");


  tft.setCursor(5, 100);
  tft.println(info);

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
// fonction Callback pour la réception d'un nouveau mot de passe depuis le topic correspondant
void mqttCallback(char *topic, uint8_t *payload, unsigned int length)
{
        int payLoad[] = {0, 0, 0, 0};        
        if (length == 4){
                payLoad[0] = int((char)payload[0]) - 48;
                payLoad[1] = int((char)payload[1]) - 48;
                payLoad[2] = int((char)payload[2]) - 48;
                payLoad[3] = int((char)payload[3]) - 48;

                {
                        if (int((char)payLoad[1]) >= 0 && int((char)payLoad[1]) <= 9)
                        {
                                if (int((char)payLoad[2]) >= 0 && int((char)payLoad[2]) <= 9)
                                {
                                        if (int((char)payLoad[3]) >= 0 && int((char)payLoad[3]) <= 9)           // si la donnée recue par MQTT est bonne, on l'enregistre en tant que nouveau mdp
                                        {
                                                mdpPorte[0] = payLoad[0];
                                                mdpPorte[1] = payLoad[1];
                                                mdpPorte[2] = payLoad[2];
                                                mdpPorte[3] = payLoad[3];
                                        }
                                }
                        }
                    }
                }
}
//Initialise ou rétablie la connexion MQTT
void initMqtt(){ 
  mqttClient.setServer(mqttServer, mqttPort);
  cptTentativeRecoMqtt = 0;
  while (!mqttClient.connected())
  {

          if (mqttClient.connect(idCapteur, mqttUser, mqttPassword))
          {
                etatMqtt = "OK";
          }
          else
          {
                etatMqtt = "KO";
                delay(2000);
          }
          mqttClient.setCallback(mqttCallback);
          cptTentativeRecoMqtt = cptTentativeRecoMqtt + 1;
          if (cptTentativeRecoMqtt == 3)
          {
                cptTentativeRecoMqtt = 0;
                break;
          }
  }
  bool var = false;
  while (!var)
  { var = mqttClient.subscribe("home/security/entry_code", 1); }
    GestionIHM("");
    mqttClient.loop(); 
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

//Fct permettant de selectionner un digit sur le 4*7Segments
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
//Active les segments correspondant au chiffre en paramètre
void chiffre(int chiffre){
        switch (chiffre)
        {

        case -1:                                       //éteint le digit, aucun segment allumé
                digitalWrite(pinSegmentA, LOW);
                digitalWrite(pinSegmentB, LOW);
                digitalWrite(pinSegmentC, LOW);
                digitalWrite(pinSegmentD, LOW);
                digitalWrite(pinSegmentE, LOW);
                digitalWrite(pinSegmentF, LOW);
                digitalWrite(pinSegmentG, LOW);
                break;

        case 0:                                         // active les segments pour "0"
                digitalWrite(pinSegmentA,HIGH);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,HIGH);
                digitalWrite(pinSegmentE,HIGH);
                digitalWrite(pinSegmentF,HIGH);
                digitalWrite(pinSegmentG,LOW);
                break;

        case 1:                                         // active les segments pour "1"
                digitalWrite(pinSegmentA,LOW);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,LOW);
                digitalWrite(pinSegmentE,LOW);
                digitalWrite(pinSegmentF,LOW);
                digitalWrite(pinSegmentG,LOW);
                break;
        case 2:                                         // active les segments pour "2"
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
        case 9:                                         // active les segments pour "9"
                digitalWrite(pinSegmentA,HIGH);
                digitalWrite(pinSegmentB,HIGH);
                digitalWrite(pinSegmentC,HIGH);
                digitalWrite(pinSegmentD,HIGH);
                digitalWrite(pinSegmentE,LOW);
                digitalWrite(pinSegmentF,HIGH);
                digitalWrite(pinSegmentG,HIGH);
                break;
                
        default:                                        // barre central 
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
//Fct appelée quand l'utilisateur appuie sur le btn de validation du mot de passe
void verificationMdp(){
        if (valeur7segment[0]==mdpPorte[0] && valeur7segment[1]==mdpPorte[1] && valeur7segment[2]==mdpPorte[2] &&valeur7segment[3]==mdpPorte[3]) {
              codeValide = 1;              
        }else{
               codeValide = -1;                     
        }
}
//Permet d'incrementer la valeur du digit en paramètre
void incrementerChiffre(){
 switch (digitActuel)
 {

 case 1:
        valeur7segment[0] = valeur7segment[0] + 1;
        if (valeur7segment[0] > 9 ) {valeur7segment[0] = 0;
        }
        break;
 case 2:
        valeur7segment[1] = valeur7segment[1] + 1;
        if (valeur7segment[1] > 9 ) {
                valeur7segment[1] = 0; 
        }
        break;
 case 3:
        valeur7segment[2] = valeur7segment[2] + 1;
        if (valeur7segment[2] > 9 ) {
                valeur7segment[2] = 0;
        }
        break;
 case 4:
        valeur7segment[3] = valeur7segment[3] + 1;
        if (valeur7segment[3] > 9 ) {
                valeur7segment[3] = 0;
        }
        break;
 
 default:
         break;
 }


}
//passe au digit suivant
void nextDigit(){
        digitActuel = digitActuel + 1;
        if (digitActuel > 4 ){
                digitActuel = 1;
        }
        delay(400);
}

// Configuration des pins du 4*7Segments
void init7Segment(){
// Config pins 7Segement
 pinMode(pinDigitA0, OUTPUT);
 pinMode(pinDigitA1, OUTPUT);
//----------------- Pins 7 segments
 pinMode(pinSegmentA, OUTPUT);
 pinMode(pinSegmentB, OUTPUT);
 pinMode(pinSegmentC, OUTPUT);
 pinMode(pinSegmentD, OUTPUT);
 pinMode(pinSegmentE, OUTPUT);
 pinMode(pinSegmentF, OUTPUT);
 pinMode(pinSegmentG, OUTPUT);
}
//Configure les boutons poussoirs 
void initBouton(){

  //------- Pins bouttons
 pinMode(pinLedVert,OUTPUT);
 pinMode(pinLedRouge,OUTPUT);
 pinMode(pinBtnValiderMdp,INPUT);
 pinMode(pinBtnDigitSuivant,INPUT);
 pinMode(pinBtnIncrementerChiffre,INPUT); 
 attachInterrupt(pinBtnDigitSuivant,nextDigit,RISING);
 attachInterrupt(pinBtnValiderMdp,verificationMdp,RISING);       
 attachInterrupt(pinBtnIncrementerChiffre,incrementerChiffre,RISING);
}


class MyClientCallback : public BLEClientCallbacks {
        void onConnect(BLEClient* pclient)
        {}
        void onDisconnect(BLEClient *pclient)
        {}
} ;

bool connectToServer()
{
        BLEClient *pClient = BLEDevice::createClient();
        pClient->setClientCallbacks(new MyClientCallback());
        // Connect to the remove BLE Server.
        pClient->connect(myDevice); // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)

        // Obtain a reference to the service we are after in the remote BLE server.
        BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
        if (pRemoteService == nullptr)
        {       pClient->disconnect();
                return false;
        }
        // Obtain a reference to the characteristic in the service of the remote BLE server.
        pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
        if (pRemoteCharacteristic == nullptr)
        {
                pClient->disconnect();
                return false;
        }
        // Read the value of the characteristic.
        if (pRemoteCharacteristic->canRead())
        {
                std::string value = pRemoteCharacteristic->readValue();        
        }

        if (pRemoteCharacteristic->canNotify())
               // pRemoteCharacteristic->registerForNotify(notifyCallback);

        connected = true;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
        /**
         * Called for each advertising BLE server.
         */
        void onResult(BLEAdvertisedDevice advertisedDevice)
        {
               // We have found a device, let us now see if it contains the service we are looking for.
                if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
                {
                        GestionIHM("BLE");
                        BLEDevice::getScan()->stop();
                        myDevice = new BLEAdvertisedDevice(advertisedDevice);
                        doConnect = true;
                        doScan = true;
                } 
        }         
};                

void setup()
{
        // Initialisation des peripheriques
        Serial.begin(115200);
        delay(1000);
        initScreen();
        GestionIHM("");
        initWifiConnection();
        init7Segment();
        initBouton();
        // Partie BLE, partiellement fonctionnelle
        BLEDevice::init("ModuleVerrou");
        BLEScan* pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
        pBLEScan->setInterval(1400);
        pBLEScan->setWindow(500);
        pBLEScan->setActiveScan(true);
}

void loop() {
   
  if (!mqttClient.connected())
  {                                                      // vérifie l'état de la connexion MQTT
          etat = 0;
          etatMqtt = "KO";
  }                                                      // si le client mqtt est déconnecté
  else{
        etat = 1;
        etatMqtt = "OK";
        mqttClient.loop();
        }
  if (WiFi.status() != WL_CONNECTED){etatWifi = "KO";}   // vérifie l'état de la connexion WiFi
  else{etatWifi = "OK";}

  if (etatMqtt == "KO")
  {
         // etat = 2;
  }

  switch (etat)                                         // exécute un ensemble de routine en fonction de l'état du système
    {
        case 0:                                         // si état=0, client mqtt est déconnecté
                if (WiFi.status() != WL_CONNECTED){     // Contrôle état connection Wifi, reconnection si déconnecté
                WiFi.reconnect();
                }
                GestionIHM("");
                initMqtt();                                 // tentative de (re)connection du client mqtt au brooker
        break;

        case 1:                                              // si le MQTT et la WiFi sont connectés, on affiche les 
                chiffre(-1);                                 // éteint les segments   
                digitalWrite(pinDigitA1, LOW);               // selectionne le premier digit   
                digitalWrite(pinDigitA0, LOW);
                delayMicroseconds(1);
                chiffre(valeur7segment[0]);                  // allume les segments correspondant à la valeur en paramètre   
                delay(5);

                chiffre(-1);
                digitalWrite(pinDigitA0, HIGH);              // selectionne le deuxième digit
                digitalWrite(pinDigitA1, LOW);
                delayMicroseconds(1);
                chiffre(valeur7segment[1]);
                delay(5);

                chiffre(-1);
                digitalWrite(pinDigitA1, HIGH);              // selectionne le troisième digit
                digitalWrite(pinDigitA0, LOW);
                delayMicroseconds(1);
                chiffre(valeur7segment[2]);
                delay(5);

                chiffre(-1);
                digitalWrite(pinDigitA0, HIGH);               // selectionne le quatrième digit
                digitalWrite(pinDigitA1, HIGH);
                delayMicroseconds(1);
                chiffre(valeur7segment[3]);
                delay(5);

                if (codeValide == 1)                                     // si l'utilisateur à validé un code correct,
                {                                                        // une interruption est attachée au bouton de validation qui appelle une fonction qui change la valeur codeValide
                        codeValide = 0;
                        digitalWrite(pinLedVert,HIGH);
                        for(int i=0;i<400;i++){                         // affiche "OK" sur le 7segments si le code entré est bon
                                chiffre(-1);
                                digitalWrite(pinDigitA0, HIGH);          // digit 2
                                digitalWrite(pinDigitA1, LOW);
                                delayMicroseconds(1);
                                chiffre(0);
                                delay(6);

                                chiffre(-1);
                                digitalWrite(pinDigitA1, HIGH);           // digit 3
                                digitalWrite(pinDigitA0, LOW);
                                delayMicroseconds(1);
                                digitalWrite(pinSegmentA, LOW);
                                digitalWrite(pinSegmentB, HIGH);
                                digitalWrite(pinSegmentC, HIGH);
                                digitalWrite(pinSegmentD, LOW);
                                digitalWrite(pinSegmentE, HIGH);
                                digitalWrite(pinSegmentF, HIGH);
                                digitalWrite(pinSegmentG, HIGH);
                                delay(6);
                                chiffre(-1);
                                
                        }
                        for(int i=0;i<4;i++){                              // envoi ordre d'ouverture sur le topic correspondant
                                mqttClient.publish("home/living_room/sensor_entry_door_authentification_mdp/authentification", "authorized");
                        }   
                        digitalWrite(pinLedVert, LOW);
                        codeValide = 0;
                }
                if (codeValide == -1)                                   // si l'utilisateur à validé un mauvais code
                {
                        codeValide = 0;
                        digitalWrite(pinLedRouge, HIGH);
                        for (int i=0;i<30;i++)                          // fait clignoter les digits
                        {       
                                delay(50);
                                chiffre(-1);
                                digitalWrite(pinDigitA0, LOW); // digit 2
                                digitalWrite(pinDigitA1, LOW);
                                delayMicroseconds(1);
                                chiffre(0);
                                delay(6);
                                chiffre(-1);
                                digitalWrite(pinDigitA0, HIGH); // digit 2
                                digitalWrite(pinDigitA1, LOW);
                                delayMicroseconds(1);
                                chiffre(0);
                                delay(6);
                                chiffre(-1);
                                digitalWrite(pinDigitA0, LOW); // digit 2
                                digitalWrite(pinDigitA1, HIGH);
                                delayMicroseconds(1);
                                chiffre(0);
                                delay(6);
                                chiffre(-1);
                                digitalWrite(pinDigitA0, HIGH); // digit 2
                                digitalWrite(pinDigitA1, HIGH);
                                delayMicroseconds(1);
                                chiffre(0);
                                
                        }
                        digitalWrite(pinLedRouge, LOW);
                        codeValide = 0;
                }

        break;

        case 2:                                 // Partie BLE
                GestionIHM("ble");
                delay(2000);

                pBLEScan->start(5, false);
                if (doConnect == true){
                        if (connectToServer()){
                                GestionIHM("ble ok");
                        }
                        doConnect = false;
                }

                if (connected){
                        String newValue = "OK";
                        // Set the characteristic's value to be the array of bytes that is actually a string.
                        pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
                }
                else if (doScan){
                        BLEDevice::getScan()->start(0); // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
                }
                delay(1000);
        break;

        default:
        break;
   }
}
