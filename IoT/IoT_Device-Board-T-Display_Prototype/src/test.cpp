#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


#define CONNECTION_TIMEOUT 10
    // Pins utilisés sur la carte

    ///_____
const char *ssid = "SFR_DDA8"; //"Domotique";// identifiant réseau//"SFR_DDA8";
const char *password = "3vsk72pjpz5fkd69umkz"; //"3vsk72pjpz5fkd69umkz";   // mot de passe réseau
const char *mqttServer = "192.168.1.89"; // addresse IP du brooker Mqtt
const int mqttPort = 1883;                 // port de connection mqtt
const char *mqttUser = "";
const char *mqttPassword = "";
const char *idCapteur = "Server"; // identifiant du module
const int mdpPorte = 2234;          // mot de passe de la porte

int etat = -1;                                // variable representant l'etat du capteur : (-1) --> Wifi deconnecté
volatile int digitActuel = 1;                 // variable correspondant au digit à allumer, on demarre positionné sur le digit 1
volatile int valeur7segment[] = {1, 2, 3, 4}; // valeur affiché sur le 7Segment
int codeValide = 0;

char *etatWifi = ""; // paramètre permettant de récuperer l'état de connection Wifi
char *etatMqtt = ""; // paramètre permettant de récuperer l'état de connection MQTT
char *characteristiqueValeur ;

const char *var = "";
std::string var1;

    ///____ Appel classes
    TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup_Select.h
WiFiClient espClient;
PubSubClient mqttClient(espClient);

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

void GestionIHM(char *info)
{
        tft.fillScreen(TFT_WHITE);
        tft.setCursor(15, 20);
        tft.println(idCapteur); // mqttClient.state());
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

void GestionIHM(const char *info)
{
        tft.fillScreen(TFT_WHITE);
        tft.setCursor(15, 20);
        tft.println(idCapteur); // mqttClient.state());
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

void wifiConnected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info)
{
        etatWifi = "OK";
}

void wifiGotIP(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info)
{
        tft.fillScreen(TFT_WHITE);
        tft.setCursor(5, 40);
        tft.println(WiFi.localIP());
        delay(1000);
        etatWifi = "OK";
}

void wifiDisconnected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info)
{
        etatWifi = "KO";
        int compteur_conn = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
                compteur_conn++; // redemarre la carte apres 30s, equivalent de 150 tentatives de connection
                if (compteur_conn >= 5 * CONNECTION_TIMEOUT)
                {
                        ESP.restart();
                }

                WiFi.begin(ssid, password);
                delay(100);
        }
}

// Se connecte à un réseau WiFI
// Gestion WiFi par les event de la connection
void initWifiConnection()
{
        WiFi.mode(WIFI_STA);                                           // mode station
        WiFi.onEvent(wifiConnected, SYSTEM_EVENT_STA_CONNECTED);       // si l'état de la co Wifi passe à connected, appel la fct wifiConnected
        WiFi.onEvent(wifiGotIP, SYSTEM_EVENT_STA_GOT_IP);              // // si la carte recoit une @IP appel la fct wifiGotIP
        WiFi.onEvent(wifiDisconnected, SYSTEM_EVENT_STA_DISCONNECTED); // si l'état de la co Wifi passe à disconnected, appel la fct wifiDisconnected, permet de se reconnecter automatiquement

        WiFi.begin(ssid, password);
}
// fonction Callback pour publication sur le topic mdp
void mqttCallback(char *valeur, uint8_t *var, unsigned int nb)
{
}

void initMqtt()
{
        mqttClient.setServer(mqttServer, mqttPort);
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
        }
        bool var = false;
        while (!var)
        {
                var = mqttClient.subscribe("home/security/entry_code", 1);
        }
        GestionIHM("");
        mqttClient.loop();
}
/*
void gestionBouton(){
  // la pin 4 correspond à la backlight
  if (digitalRead(4)!=LOW){digitalWrite(4,LOW);}
  else{digitalWrite(4,HIGH);}
}
*/

// Fonction pour publier un float sur un topic
void mqtt_publish_float(String topic, float t)
{
        char top[topic.length() + 1];
        topic.toCharArray(top, topic.length() + 1);
        char t_char[50];
        String t_str = String(t);
        t_str.toCharArray(t_char, t_str.length() + 1);
        mqttClient.publish(top, t_char);
}


void setup()
{
        // Init peripheriques
        Serial.begin(115200);
        delay(1000);
        initScreen();
        GestionIHM("");
        initWifiConnection();

        BLEDevice::init("Server");
        BLEServer *pServer = BLEDevice::createServer();
        BLEService *pService = pServer->createService(SERVICE_UUID);
        BLECharacteristic *pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

        pCharacteristic->setValue("Hello World");
        pService->start();
        // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();        
}

void loop()
{

        if (!mqttClient.connected()){
                etat = 0;
                etatMqtt = "KO";
        } // si le client mqtt est déconnecté
        else{
                etat = 1;
                etatMqtt = "OK";
                mqttClient.loop();
        }
        if (WiFi.status() != WL_CONNECTED){
                etatWifi = "KO";
        }
        else{
                etatWifi = "OK";
        }
       // pCharacteristic->getValue();

        

        switch (etat){
                
        case 0: // si état=0, client mqtt est déconnecté
                if (WiFi.status() != WL_CONNECTED)
                { // Contrôle status connection Wifi, reconnection si déconnecté
                        WiFi.reconnect();
                }
                GestionIHM("");
                initMqtt(); // tentative de (re)connection du client mqtt au brooker
                break;

        case 1:

                if (codeValide == 1)
                {
                       
                        for (int i = 0; i < 4; i++)
                        { // envoi ordre d'ouverture
                                mqttClient.publish("home/living_room/sensor_entry_door_authentification_mdp/authentification", "authorized");
                        }
                        
                        delay(100);
                      
                        delay(100);
                }
                if (codeValide == -1)
                {
                        codeValide = 0;
                      
                }
                
              //  var = pCharacteristic->getValue().c_str();
              //  var1 = var;
              //  GestionIHM(var);
           //   Serial.println(pCha)

        default:
                break;
        }
}

