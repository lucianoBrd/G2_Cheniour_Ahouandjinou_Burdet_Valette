# Projet Domotique

**Supervisé par Raphaël Leber et Fabrice Jumel.**

Notre projet permet de visualiser, faciliter et administrer différents éléments de sa maison : température, humidité, porte-volet, chauffage, VMC...

Lien de la vidéo de démonstration du jalon 1
- https://youtu.be/Neju9fB2eeY

Lien de la vidéo de démonstration du jalon 2
- https://youtu.be/iFAQ2z5Xobs

Lien de la vidéo de publicité attractive
- https://youtu.be/czC0UCgZDRA

Voici le lien de notre application WEB (API + Dashboard) :
- https://work.lucien-brd.com/

Lien du Dashboard Power BI:
- https://app.powerbi.com/groups/me/reports/f1663c79-8891-434a-b447-44981bdea118/ReportSection

1. Introduction
	1. Schéma global
	2. Résultats attendus
	3. Fonctionnalités de base
	4. Liste des éléments de base de la pièce
	5. Évolutions possibles
	6. Matériel
	7. Langage et technologie
2. Activités réalisées
	1. Serveur WEB
	2. Robot Pepper
	3. Serveur Global
	4. IOT
	5. Power BI

# 1. Introduction

## Membres du groupe

- Aykel Cheniour - aykel.cheniour@cpe.fr
- Valentin Valette - valentin.valette@cpe.fr
- Enagnon Farell Grazina Ahouandjinou - ah.farell@gmail.com
- Lucien Burdet - lucien.burdet@cpe.fr

## 1.1. Schéma global

![Schema](schema/schema.png)

## 1.2. Résultats attendus

Notre solution doit permettre la gestion globalisée de la maison. 
D'une part de manière autonome, en fonction des entrées garder une certaine sortie : garder une température constante. En somme, s'assurer que l'ordre donné soit réalisé.
D'autre part de façon contrôlée, c'est-à-dire gérer via la voix, ou de manière applicative...

## 1.3. Fonctionnalités de base

- Gestion de la température de chaque pièce **Jalon 1**
- Gestion de la luminosité de chaque pièce **Jalon 1**
- Contrôle de l'ouverture de la porte d'entrée par détection faciale **Jalon 2**
- Consultation en temps réel de l'état des équipements connectés **Jalon 1**
- Gestion de la maison via Pepper  **Jalon 1**

## 1.4. Liste des éléments de base de la pièce
- VMC **Jalon 1**
- Température **Jalon 1**
- Humidité **Jalon 1**
- Volet **Évolution Future**
- Porte **Jalon 1**
- Prise **Évolution Future**
- Luminosité **Jalon 2**
- Chauffage **Jalon 2**

## 1.5. Évolutions possibles

- Mode préprogrammé 
	- Surveillance : détecte les mouvements dans la maison et effectue une notification **Jalon 2**
	- Fête : met de la musique et les lumières de la pièce clignotent. **Jalon 2**
- Application Android de gestion **Évolution Future**
- Dashboard de visualisation **Jalon 2**
- Intégration Google Home **Évolution Future**
- Ajout de code de vérification/confirmation pour l’ouverture de la porte d’entrée **Jalon 2**
- Cryptage des données / Sécurisation du serveur **Évolution Future**
- Contrôle des prises de la maison **Évolution Future**
- Intégration d’un capteur luminosité **Jalon 2**
- Dashboard PowerBi **Jalon 2**

## 1.6. Matériel

- ESP32 : TTGO-Camera, TTGO-Display...
	- Capteur : Température, Humidité, Luminosité (photorésistance), caméra
	- Servo-moteur 
	- Leds 
	- Résistances(10K, 330 Ohms)
	- Boutons poussoirs
	- Condansateurs (150nF)
	- Démultiplexeur (74HC138N)
- Serveur Python
- Serveur WEB avec base de données
- Robot Pepper 

## 1.7. Langage et technologie

- Python
- C++ - PlateformIO
- Flask 
- MQTT 
- SQL
- HTML/CSS/JS
- PHP - Symfony

# 2. Activités réalisées

## 2.1. Serveur WEB

Pour la partie WEB nous avons développé une application en PHP - Symfony.
Symfony est un framework MVC libre écrit en PHP.

Notre application propose une API permettant de gérer les éléments de la base de données.
De plus, l'application propose un dashboard afin d'administrer aisément depuis n'importe quel appareil.

Le lien de l'application WEB (API + Dashboard) est le suivant : https://work.lucien-brd.com/

Voici le lien du readme permettant de mettre en place le serveur WEB : ```web_server_api_dashboard/readme.md```

Vidéo de mise en place du serveur WEB : https://youtu.be/J7XK7m5FHh4

### 2.1.1. Base de données

La base de données est en MySQL. Voici son schéma :

![Schema BDD](schema/database/schema_database.png)

### 2.1.2. Dashboard

Voici des captures d'écran du dashboard.
Celui-ci permet d'ajouter, éditer ainsi que supprimer les différents composants de la base de données.

- dashboard ![dashboard](schema/web_server_api_dashboard/dashboard.png)
- home ![home](schema/web_server_api_dashboard/home.png)
- action ![action](schema/web_server_api_dashboard/action.png)
- element ![element](schema/web_server_api_dashboard/element.png)
- create_element ![create_element](schema/web_server_api_dashboard/create_element.png)
- mode ![mode](schema/web_server_api_dashboard/mode.png)
- edit_mode ![edit_mode](schema/web_server_api_dashboard/edit_mode.png)
- room ![room](schema/web_server_api_dashboard/room.png)
- type ![type](schema/web_server_api_dashboard/type.png)
- value ![value](schema/web_server_api_dashboard/value.png)
- delete_value ![delete_value](schema/web_server_api_dashboard/delete_value.png)

### 2.1.3. API

Vous pouvez retrouver la documentation de l'API à cette adresse : https://work.lucien-brd.com/api

Voici les différentes routes de l'API :

![API](schema/api.png)


## 2.2. Robot Pepper

Voici le lien du readme permettant de mettre en place le robot Pepper : ```pepper/readme.md```

Vidéo de mise en place du Pepper : https://youtu.be/w5FxUbHlQkk

### 2.2.1. Serveur

Le serveur est codé en Python à l'aide de la librairie NAOQI. 
Celui-ci permet de requêter l'API du serveur WEB et de lancer un service visible par le Pepper. 
En somme, le serveur permet donc au Pepper d'avoir accès à l'API.

Le serveur doit donc être lancé sur une machine disposant d'un accès à internet.

Voici les liens entre le l'API et le serveur :

![serveur](schema/pepper/serveurrobot.png)

### 2.2.2. Pepper

Le Pepper a été développé à l'aide du logiciel Choregraphe.

Voici comment s'articulent les fonctionnalités :

- pepper ![pepper](schema/pepper/pepper.png)
- pepper_init ![pepper_init](schema/pepper/pepper_init.png)
- pepper_homelist ![pepper_homelist](schema/pepper/pepper_homelist.png)
- pepper_home1 ![pepper_home1](schema/pepper/pepper_home1.png)
- pepper_home2 ![pepper_home2](schema/pepper/pepper_home2.png)
- pepper_absencemode ![pepper_absencemode](schema/pepper/pepper_absencemode.png)
- pepper_fetemode ![pepper_fetemode](schema/pepper/pepper_fetemode.png)

Vous pouvez également trouver des captures d'écran de notre solution du logiciel Choregraphe dans le dossier suivant ```schema/pepper/screen/```

### 2.2.3. Dashboard

Le dashboard est développé en HTML/CSS/JS. 
Il permet d'afficher le flux de la caméra lorsqu'un Home est choisi.
De plus, il permet de choisir un Home, monitorer celui-ci et également choisir le mode.

Voici des captures d'écran du dashboard.

- init ![init](schema/pepper/dashboard/init.png)
- homelist ![homelist](schema/pepper/dashboard/homelist.png)
- home 
	![home1](schema/pepper/dashboard/home1.png)
	![home2](schema/pepper/dashboard/home2.png)
	![home3](schema/pepper/dashboard/home3.png)
	![home4](schema/pepper/dashboard/home4.png)
- modefete ![modefete](schema/pepper/dashboard/modefete.png)
- modeabsence ![modeabsence](schema/pepper/dashboard/modeabsence.png)

## 2.3. Serveur Global

Le serveur global permet de mettre en relation les données de la base de données à l'aide de l'API et les données des éléments IOT via le server MQTT. Ainsi, il va mettre en base de données les valeurs récolté par les éléments IOT, traiter les actions à réaliser sur les éléments IOT (par exemple : ouvrir la porte, éteindre la lumière ou encore atteindre une certaine température) et jouer des scénarios/modes (par exemple : le mode fête ou encore le mode absent).

Voici l'algoritmhe qui résume le fonctionnement global du serveur :
- ![diagram_server_domotique](schema/server_global/diagram_server_domotique.drawio.png)

Voici l'exemple du CLI du serveur lorsque le projet tourne : 
- ![cli_server](schema/server_global/capture_sheel_server_global.PNG)

### 2.3.1. Liste des topic mqtt mis en place

- home/living_room/sensor_temperature/temperature
- home/living_room/sensor_humidity/humidity
- home/living_room/sensor_luminosity/luminosity
- home/living_room/sensor_entry_door_authentification_face/authentification
- home/living_room/sensor_entry_door_authentification_mdp/authentification
- home/living_room/actuator_vmc/state
- home/living_room/actuator_lum/state
- home/living_room/actuator_entry_door/state
- home/living_room/actuator_heating/state
- home/security/entry_code

### 2.3.2. Liste des topic mqtt à mettre en place dans les évolutions futures

- home/living_room/actuator_shutter/state

## 2.4. IOT


### 2.4.1. Reconnaissance faciale
Shéma global de la reconnaissance faciale :
![Reconnaissance facial Shéma](schema/IOT/Reconnaissance%20facial%20Shéma.PNG)

#### 2.4.1.1. ESP32Cam

Vidéo de mise en place de ESP32Cam : https://youtu.be/iwUzKX_GJJs

#### 2.4.1.2. Serveur reconaissance faciale

Le serveur de reconnaissance faciale accèdes au flux d'image pour les comparer avec des visages enregistrés.
De plus, il est possible d'enregistrer des visages a votre guise.
Quand un visage est reconnu une authorisation est envoyée via le MQTT.

Diagramme reconnaissance faciale :
![Reconnaissance faciale Diagram](schema/IOT/Reconnaissance%20faciale%20Diagram.PNG)

### 2.4.2. Module chauffage
Le module de chauffage est composer d'un ESP32, une led et une résistance qui simule le chauffage.

Shéma électrique du module: 
![Module Chauffage](schema/IOT/Module%20Chauffage.PNG)

### 2.4.3. Module Lumière
Le module de chauffage est composer d'un ESP32, 6 led de couleur différente et 3 résistance pour réguler l'intesité.

Shéma électrique du module: 
![Module Luminausité](schema/IOT/Module%20Luminausité.PNG)

### 2.4.4. Module VMC
Le Module VMC est composé d'un ESP32 et un servo moteur commandé en vitesse.

Shéma électrique du module: 
![Module VMC](schema/IOT/Module%20VMC.PNG)
### 2.4.5. Module Vérrou
Le Module VMC est composé d'un ESP32 et un servo moteur commandé en position.

Shéma électrique du module: 
![Module Vérrou](schema/IOT/Module%20Vérrou.PNG)
### 2.4.6. Capteur de Température et d'humidité
Le capteur de température et d'humidité a pour rôle de mesurer la température et l'humidité ambiante et les publie par MQTT sur le serveur central.

Shéma électrique du module: 
![Capteur de Température et d'humidité](schema/IOT/CapteurTemp-HumSchema.png)

Organigramme du capteur de température et d'humidité: 
![Digicode](schema/IOT/CapteurTemp-HumOrganigramme.png)

### 2.4.7. Capteur de luminosité
Le capteur de luminosité mesure la luminosité ambiante et la publie par MQTT sur le serveur central.

Shéma électrique du module: 
![Capteur de luminosité](schema/IOT/CapteurLuminositeSchema.png)

Organigramme du capteur de luminosité: 
![Digicode](schema/IOT/CapteurLumiOrganigramme.png)

### 2.4.8. Digicode
Le digicode permet à l'utilisateur de saisir un code à 4 chiffres afin de rentrer dans la maison.

Shéma électrique du digicode: 
![Digicode](schema/IOT/DigicodeSchema.png)

Organigramme du digicode: 
![Digicode](schema/IOT/DigicodeOrganigramme.png)

## 2.5. Power BI 

Power BI est un outils qui permet la récupération, traitement et affichage des données, Nous avons utilisés les point d'entrés suivant de l'API : 
- https://work.lucien-brd.com/api/homes
- https://work.lucien-brd.com/api/values

Celat nous a permit d'avoir les données sous la forme suivante : 

![PowerBI_Data_structure](schema/PowerBI/PowerBI_Data_structure.PNG)

Page1 Permet d'avoir les dernières valeurs des actuators et sensors:

![Page1_PowerBI](schema/PowerBI/Page1_PowerBI.PNG)

Page2 Permet d'avoir les courbes en fonction du temps des valeurs de sensors:

![Page2_PowerBI](schema/PowerBI/Page2_PowerBI.PNG)



