# Infos utiles / Configuration
        - Choisir le framework "arduino" pour le projet
        - Telecharger les librairies necessaire depuis le magasin Platformio
        - Rajouter dans le .ini "board_build.partitions = no_ota.csv" permet d'éviter l'erreur "ram overflowed"
        - Modifer le fichier "User_Setup_Select.h" et décommenter le "../Setup25_TTGO_T_Display.h"
        - Commentez tout le fichier setup
        - Driver "ST7789_DRIVER" tft pour la Board T-Display
        

## Commande Git utile
        - git branch
        - git branche nomBranche
        - git checkout nomFiles : remets les/le fichier(s) nomFiles dans l'état du dépôt

## Mosquitto .conf
        - allow_anonymous true
        - listener 1883

## Documentation
- https://geekelectronick.com/esp32-customizing-partition-in-platformio/
- https://docs.platformio.org/en/latest/projectconf/index.html

WiFi 
- https://learn.upesy.com/fr/programming/esp32/arduino-code/esp32_connect_wifi_AP.html#:~:text=L'ESP32%20poss%C3%A8de%202%20modes,ESP32%20peut%20acc%C3%A9der%20%C3%A0%20Internet.

Mqtt
 - https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
 - https://techtutorialsx.com/2017/04/24/esp32-publishing-messages-to-mqtt-topic/
 - https://mosquitto.org/man/mosquitto_sub-1.html

Build
- https://docs.platformio.org/en/latest/projectconf/section_env_build.html#src-filter

Pins ESP32
 - https://jensd.dk/edu/doc/esp32/ttgo-t.html
 - https://jensd.dk/edu/doc/esp32/ttgo-t-pins.jpg
 - https://www.electrorules.com/esp32-pinout-reference/

4*7Segnment
 - https://osoyoo.com/wp-content/uploads/2017/08/4-digit-7-segment-LED-display-pinout.png

74HC138N
 - https://html.alldatasheet.com/html-pdf/15540/PHILIPS/74HC138N/1003/4/74HC138N.html