Lancer le porjet :
```
symfony serve -d --no-tls
```

Arreter le projet :
```
symfony server:stop
```

Mettre en place la base de données :
```
php bin/console doctrine:database:create
php bin/console doctrine:schema:update --force
```