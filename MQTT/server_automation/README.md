# Script server domotique 

## Installation

### Pré-requis
- Installation de python : si vous n'avez pas python sur votre machine vous pouvez le télécharger via le lien suivant : https://www.python.org/downloads/

- Insallation du package `virtualenv` : ouvrir un terminal et exécuter la commande `pip install virtualenv`

- Création d'un environnement virtuel : il est fortement recommandé d'utiliser un environnement virtuel pour exécuter en sécurité le projet, pour cela exécuter la commande suivante dans la racine du projet `virtualenv venv`

- Activer le venv : `./venv/Scripts/activate.ps1`

- Installez les dépendances : `pip install -r .\requirements.txt`

### Exécution du script

Pour exécuter le script lancer la commande suivante : `py domotique_automatisation.py`.

## Descriptif du script

Ce script permet de gérer les différente entité de la maison (via un server MQTT) et de paramétrer des scénario entre elles. Par exemple, lorsque la température (capteur de température) descend en dessous d'un seuil on active le chauffage. 

**Remarque** : Vous pouvez retrouver plus d'informations sur les différentes via la documention HTML suivante : [documentation des classes](https://work.lucien-brd.com/doc/)



