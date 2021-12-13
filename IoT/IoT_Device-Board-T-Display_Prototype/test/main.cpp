#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <DHT.h>

#define CONNECTION_TIMEOUT 10
#define DHTPIN 2         // broche DATA du capteur est reliee 
#define DHTTYPE DHT11     // Definit le type de capteur utilise
///_____
const char* ssid = "SFR_DDA8";    //"Redmi Note 7"; // 
const char* password = "3vsk72pjpz5fkd69umkz";  // dallez94
const char* mqttServer = "192.168.1.89";  //"broker.hivemq.com";//IPAddress my_IPAddress(192,168,43,222);
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

const char* temp = "";
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
// Gestion WiFi par les event de la co
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
  while (WiFi.status() != WL_CONNECTED){}
  
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
  
    if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
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
  
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  initScreen();
  initWifiConnection();
  initMqtt();
// Config pins
  pinMode(2, OUTPUT);
  pinMode(12, OUTPUT);
  
  digitalWrite(2, HIGH);

  //Config PWM
  int pwmChannel = 0;  // channel de O-15
  int pwmFreq = 1000; 
  int pwmResolution = 8;  // résolution de la largeur d’impulsion entre 1 et 16 bits
  int pwmPin = 12;

  // Configuration du canal 0 avec la fréquence et la résolution choisie
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);

   // Assigne le canal PWM au pin 36
  ledcAttachPin(pwmPin, pwmChannel);
 
  // Créer la tension en sortie choisi
//  ledcWrite(pwmChannel, 25); //1.65 V

  mqttClient.loop();
  mqttClient.setKeepAlive(300);
  mqttClient.setSocketTimeout(300);


  dht.begin();
  }

void loop() {
 
  
  if (!mqttClient.connected()){
      initMqtt();
  }
   
  float str =  dht.readTemperature();
  String valTemp = "";
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(5, 90);
  tft.println(str);//mqttClient.state());
  valTemp = valTemp.concat(str);
// char* tp = valTemp.toCharArray();
    mqttClient.publish("esp/test", "Hello from ESP32");
 // mqttClient.publish("esp/temp",valTemp );
  delay(5000);

}


/// note 
/* il faudra peut-être changer l'adresse mac des cartes

Def pins:

#define TFT_MOSI            19
#define TFT_SCLK            18
#define TFT_CS              5
#define TFT_DC              16
#define TFT_RST             23

#define TFT_BL          4 

PWM : all pins sauf les pins GPIO36, GPIO39, GPIO34, GPIO35
12 --> moteur

GPIO : 
2 --> led


*/

class GestionWiFi
{
private:
  /* data */
  const char* ssid;
  const char* password;

public:
  GestionWiFi(const char* ssid, const char* password);
  ~GestionWiFi();
  void connect();
 //void wifiConnected(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info);  

};

GestionWiFi::GestionWiFi(const char* ssid, const char* password)
{
   this->ssid = ssid;
   this->password = password;
}
void GestionWiFi::connect(){
   WiFi.mode(WIFI_STA); // mode station
   WiFi.onEvent(wifiConnected,SYSTEM_EVENT_STA_CONNECTED); // si l'état de la co Wifi passe à connected, appel la fct wifiConnected
   WiFi.onEvent(wifiGotIP,SYSTEM_EVENT_STA_GOT_IP);        // // si la carte recoit une @IP appel la fct wifiGotIP
   WiFi.onEvent(wifiDisconnected,SYSTEM_EVENT_STA_DISCONNECTED);  // si l'état de la co Wifi passe à disconnected, appel la fct wifiDisconnected, permet de se reconnecter automatiquement
   WiFi.begin(ssid, password);

}
GestionWiFi::~GestionWiFi()
{
}

