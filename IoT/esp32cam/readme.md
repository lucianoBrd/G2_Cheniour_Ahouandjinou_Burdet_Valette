# Procédure de lancement

Vidéo de mise en place de ESP32Cam : https://youtu.be/iwUzKX_GJJs

Afin de lancer correctement ce projet, il faut dans un premier temps vérifier que les paramètres du WIFI dans le fichier ```src/CameraWebServer.ino``` :
```
const char* ssid = "Domotique";
const char* password = "Domotique";
```

Une fois ces paramètres définis, vous pouvez lancer le projet à l'aide de Visual Studio Code, extention PlatformIO.
