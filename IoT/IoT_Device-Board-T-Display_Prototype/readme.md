# Infos utiles / Configuration
        - Choisir le framework "arduino" pour le projet
        - Telecharger les librairies necessaire depuis le magasin Platformio
        - Rajouter dans le .ini "board_build.partitions = no_ota.csv" permet d'éviter l'erreur "ram overflowed"
        - Modifer le fichier "User_Setup_Select.h" et décommenter le "../Setup25_TTGO_T_Display.h"
        - Driver "ST7789_DRIVER" tft pour la Board T-Display
        

## Commande Git utile
        - git branch
        - git branche nomBranche
        - git checkout nomFiles : remets les/le fichier(s) nomFiles dans l'état du dépôt


## Documentation
- https://geekelectronick.com/esp32-customizing-partition-in-platformio/
- https://docs.platformio.org/en/latest/projectconf/index.html