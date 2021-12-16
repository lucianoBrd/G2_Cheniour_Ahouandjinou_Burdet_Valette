# Procédure de lancement 

Avant de lancer le projet sur le robot, il faut d'abord lancer le service permettant de requêter l'API.
Pour ce faire, vous devez vous connecter au robot en ssh (```ssh nao@pepper01```) afin de récupérer son addresse IP dans le but de la mettre dans le fichier ```scripts/service.py``` (```app = qi.Application(url="tcp://134.214.50.49:9559"```)).
```
export PYTHONPATH=${PYTHONPATH}:/softwares/INFO/Pepper/pynaoqi-python2.7-2.5.5.5-linux64/lib/python2.7/site-packages
python2 scripts/service.py
```

De plus, il faut également connecter la tablette du pepper à internet, pour ce faire :
```
ssh nao@pepper01
qicli call ALTabletService._openSettings
```
Vous devez donc vous connecter en SSH au pepper et lancer la commande ```qicli```. 
Ensuite, vous devrez connecter la tablette au même réseau WIFI que le microcontroleur ESP32 de la caméra.

Une fois le service lancé, vous pouvez lancer le progamme sur le robot avec l'application Choregraphe.
