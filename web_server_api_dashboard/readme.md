Prérequis:
- Composer
- Symfony
- PHP
- MySQL

Aller dans le dossier du serveur WEB :
```
cd web_server_api_dashboard/
```

Installer les dépendances :
```
composer install
```

Mettre en place la base de données :
Modifier l'URL de la base de données dans le fichier ```.env``` : DATABASE_URL.
```
php bin/console doctrine:database:create
php bin/console doctrine:schema:update --force
```

Lancer le porjet :
```
symfony serve -d --no-tls
```

Arreter le projet :
```
symfony server:stop
```
