# MQTT Server Windows
* Connaitre l'adresse IP de l'ordinateur windows qui sera le MQTT server :
  ```
  ipconfig
  ```
* Modifier le fichier ```C:\Program Files\Mosquitto\mosquitto.conf``` :
  ```
  listener 1883 192.168.1.105 # Adresse IP de l'ordinateur windows qui sera le MQTT server
  allow_anonymous true
  ```
* Lancer le serveur :
  - Dans la barre de recherche, taper *services* puis *entrer*
  - Trouver *Mosquitto Broker*, click droit puis *Démarrer* ou *Arrêter*
* Verifier que le serveur MQTT est lancé :
  ```
  netstat
  ```
  Il faut avoir une ligne de ce type ```TCP    192.168.1.105:1883     192.168.1.103:59368    ESTABLISHED```.
* http://www.steves-internet-guide.com/install-mosquitto-windows-xp/
* https://mosquitto.org/download/