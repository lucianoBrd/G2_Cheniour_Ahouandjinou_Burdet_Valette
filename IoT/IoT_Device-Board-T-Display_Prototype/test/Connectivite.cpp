// #include <WiFi.h>
// #include <Wire.h>
// #include <Arduino.h>

// class GestionWiFi{
//  private:
//                 const char* ssid;
//                 const char* password;
                
//  public:
//         void init(const char* ssid, const char* password){
//                 this->ssid = ssid;
//                 this->password = password;
                
//         }
 
//         int connect(){
//                 WiFi.mode(WIFI_STA); // mode station
//                 WiFi.onEvent(wifiConnected,SYSTEM_EVENT_STA_CONNECTED); // si l'état de la co Wifi passe à connected, appel la fct wifiConnected
//                 WiFi.onEvent(wifiGotIP,SYSTEM_EVENT_STA_GOT_IP);        // // si la carte recoit une @IP appel la fct wifiGotIP
//                 WiFi.onEvent(wifiDisconnected,SYSTEM_EVENT_STA_DISCONNECTED);  // si l'état de la co Wifi passe à disconnected, appel la fct wifiDisconnected, permet de se reconnecter automatiquement
//                 WiFi.onEvent(init,SYSTEM_EVENT_AP_START);
//                 WiFi.begin(ssid, password);

//         }
//         void disconnect(){

//         }





// void wifiConnected(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){

// }

// void wifiGotIP(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){
//     tft.fillScreen(TFT_WHITE);
//     tft.setCursor(5, 40);
//     tft.println(WiFi.localIP());
//     delay(1000);
// }

// void wifiDisconnected(WiFiEvent_t wifi_event,WiFiEventInfo_t wifi_info){
//     int compteur_conn = 0;
//     while(WiFi.status() != WL_CONNECTED){
//        tft.fillScreen(TFT_WHITE);
//        tft.setCursor(5, 40);
//        tft.println(".......");       
//        compteur_conn++;                         // redemarre la carte apres 30s, equivalent de 150 tentatives de connection
//        if (compteur_conn >= 5*CONNECTION_TIMEOUT){
//          ESP.restart();
//        }
      
//        WiFi.begin(ssid, password); delay(100);
//     }
// }

                
                

// };
