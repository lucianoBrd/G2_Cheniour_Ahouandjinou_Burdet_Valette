Vidéo de mise en place du Pepper : https://youtu.be/w5FxUbHlQkk

Vous devez vous trouver dans le dossier ```pepper``` :
```
cd pepper/
```

# Procédure de lancement 

Premièrement, il faut connecter la tablette du pepper à internet, pour ce faire :
```
ssh nao@pepper02
qicli call ALTabletService._openSettings
```
Vous devez donc vous connecter en SSH au pepper et lancer la commande ```qicli```. 
Ensuite, vous devrez connecter la tablette au même réseau WIFI que le microcontroleur ESP32 de la caméra.

Puis, avant de lancer le projet sur le robot, il faut d'abord lancer le serveur du robot permettant de requêter l'API.

Pour ce faire, vous devez vous connecter au robot en ssh (```ssh nao@pepper02```) afin de récupérer son addresse IP ```ifconfig```.

Une fois que vous aurez l'addresse IP du robot, vous pourrez la mettre dans le fichier ```scripts/service.py``` (```app = qi.Application(url="tcp://134.214.51.43:9559"```)).
Ensuite, vous pouvez lancer le serveur :
```
export PYTHONPATH=${PYTHONPATH}:/softwares/INFO/Pepper/pynaoqi-python2.7-2.5.5.5-linux64/lib/python2.7/site-packages
python2 scripts/service.py
```

Une fois le serveur lancé, vous pouvez lancer le progamme sur le robot avec l'application Choregraphe.

# Réssumé des commandes :
```
ssh nao@pepper02
ifconfig
qicli call ALTabletService._openSettings
exit

export PYTHONPATH=${PYTHONPATH}:/softwares/INFO/Pepper/pynaoqi-python2.7-2.5.5.5-linux64/lib/python2.7/site-packages
python2 scripts/service.py
```
