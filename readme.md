# Projet Domotique

**Supervisé par Raphael Leber et Fabrice Jumel.**

Notre projet permet de visualiser, faciliter et administrer différents éléments de sa maison : température, humidité, porte-volet, chauffage, VMC...

Lien de la vidéo de démonstration du jalon 1
- https://youtu.be/Neju9fB2eeY

Voici le lien de nottre application WEB (API + Dashboard) :
- https://work.lucien-brd.com/


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
D'autre part de façon contrôlée, c'est à dire gérer via la voix, ou de manière applicative...

## 1.3. Fonctionnalités de base

- Gestion de la température de chaque pièce **Intégré dans le jalon 1**
- Gestion de la luminosité de chaque pièce **Intégré dans le jalon 1**
- Contrôle de l'ouverture de la porte d'entrée par détection facial **Intégré dans le jalon 2**
- Consultation en temps réel de l'état des équipements connectés **Intégré dans le jalon 1**
- Gestion de la maison via pepper  **Intégré dans le jalon 1**

## 1.4. Liste des éléments de base de la pièce
- VMC **Intégré dans le jalon 1**
- Température **Intégré dans le jalon 1**
- Humidité **Intégré dans le jalon 1**
- Volet **Evolution Future**
- Porte **Intégré dans le jalon 1**
- Prise **Evolution Future**
- Luminosité **Intégré dans le jalon 2**
- Chauffage **Intégré dans le jalon 2**

## 1.5. Évolutions possibles

- Mode préprogrammé (ambiance de la maison, mode chill, mode surveillance, mode absence, etc….) 
	- Surveillance : détecte les mouvements dans la maison et effectue une notification **Intégré dans le jalon 2**
	- Fête : met de la musique et les lumières de la pièce clignotent. **Intégré dans le jalon 2**
- Application Android de gestion **Evolution Future**
- DashBoard de visualisation **Intégré dans le jalon 2**
- Intégration Google Home **Evolution Future**
- Ajout de code de vérification/confirmation pour l’ouverture de la porte d’entrée **Intégré dans le jalon 2**
- Cryptage des données / Sécurisation du serveur **Evolution Future**
- Contrôle des prises de la maison **Evolution Future**
- Intégration d’un capteur luminosité **Intégré dans le jalon 2**
- Dashboard PowerBI **Intégré dans le jalon 2**

## 1.6. Matériel

- ESP32 : TTGO-Camera, TTGO-Display...
	- Capteur : Température, Humidité, Luminosité (photorésistance), caméra
	- Servo moteur 
	- Led 
	- Résistance
	- Bouton poussoir
	- Condansateur
	- Multiplexeur (74HC138N)
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

### 2.1.1. Base de données

![Schema BDD](schema/database/schema_database.png)

### 2.1.2. Dashboard

### 2.1.3. API



## 2.2. Robot Pepper
	- Serveur Robot **Python**
		- Accès à l’API 
	- Interagir avec utilisateur **Python**
	- Dashboard **HTML/CSS/JS**
		- Afficher information / dashboard de l’API 

## 2.3. Serveur Global
	- Supervision globale **Python**
	- MQTT
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
	- Reconnaissance facial 
	- Gestion Capteurs 
		- température
		- humidité 
	- Gestion des moteurs
	- Gestion simulation Prise (LED)
	- Connexion MQTT