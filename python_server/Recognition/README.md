# Script server Reconnaissance

## Installation

### Pré-requis
- Installation de python3 : si vous n'avez pas python sur votre machine vous pouvez le télécharger via le lien suivant : https://www.python.org/downloads/

- Insallation du package `virtualenv` : ouvrir un terminal et exécuter la commande `pip install virtualenv`

- Création d'un environnement virtuel : il est fortement recommandé d'utiliser un environnement virtuel pour exécuter en sécurité le projet, pour cela exécuter la commande suivante dans la racine du projet `virtualenv venv`

- Activer le venv : `./venv/Scripts/activate`

- Installez les dépendances : `pip install -r .\requirements.txt`

### Exécution du script

Avant de lancer le script modifier les adresse dans le script: `CAMERA_ADDRESS` et `BROKER_ADDRESS`
Pour exécuter le script lancer la commande suivante : `pythnon3 reco.py`.

## Descriptif du script

Le script enregistre un visage autorisé puis le compare au visage detecté sur la caméra placéé à l'entrée .Si un visage familier est detecté une autorisation est envoyée par le MQTT.





