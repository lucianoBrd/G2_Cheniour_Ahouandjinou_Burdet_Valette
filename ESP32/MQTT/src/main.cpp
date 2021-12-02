#include <Arduino.h>

// For the screen
#include <TFT_eSPI.h>

// For wifi and mqtt
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>

#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5



// Global var
const char *ssid = "Domotique";
const char *password = "Domotique";
//const char *ssid = "Luciano";
//const char *password = "123456789";
const char *mqtt_server = "broker.mqttdashboard.com";
const char *mqtt_topic_temp = "topic/temp56";

const int mqtt_port = 8000;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
    // We start by connecting to a WiFi network

    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
        {
          delay(500);

        }
        
    }


}

void callback(char *topic, byte *message, unsigned int length)
{
}

void reconnect()
{
    Serial.print("reconnect...");

    // Loop until we're reconnected
    while (!client.connected())
    {
        // Create a random client ID
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);

        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("ok");
        }
        else
        {
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}


void setup()
{
  // Monitor speed
  Serial.begin(115200);



  // Initialise wifi
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

void loop()
{
  setup_wifi();

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if (WiFi.status() == WL_CONNECTED)
  {
    char c[10];
    sprintf(c, "%.2f", 10);
    client.publish(mqtt_topic_temp, c);
    Serial.println("Sent temperature");
  }



}