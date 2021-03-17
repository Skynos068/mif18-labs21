# MIF18-Lab TP 3: Daemon

  * Nicolas Louvet, Grégoire Pichon, Université Lyon 1, LIP [email](mailto:nicolas.louvet@univ-lyon1.fr;gregoire.pichon@univ-lyon1.fr)
  * Version: 2021

## Expériences avec systemd

Sur la plupart des distributions GNU/Linux, `systemd` est le premier
programme à s'exécuter après le démarrage du noyau Linux. Il s'agit
d'un démon (processus qui "tourne" en permanence, et qui est
déconnecté de tout terminal), dont le PID est 1 : c'est donc l'ancêtre
de tous les processus du système. Son rôle est d'initialiser le
système en lançant les démons et les programmes nécessaires à son bon
fonctionnement. Il utilise pour cela des fichiers de configuration,
qui se trouvent (pour la plupart) dans `/etc/systemd/` (et encore, il
est possible que ce ne soit pas le même répertoire sur tous les
systèmes).

Jadis (il y a une dizaine d'années encore), le démon `init` jouait le
rôle que `systemd` joue actuellement : c'est pour cela que tout plein
d'articles, de cours, ou d'exos parlent encore d'`init`, mais il tend
vraiment à disparaître au profit de `systemd`. Cependant, l'un des
auteurs de ces lignes vient de vérifier que sur son Ubuntu, il reste
un lien symbolique `/sbin/init` vers `/lib/systemd/systemd` !

Pour plus de contexte, voir par exemple :

* https://fr.wikipedia.org/wiki/Systemd
* https://fr.wikipedia.org/wiki/Init#%C2%AB_init_%C2%BB_de_Unix_System_V_(SysV_init)

(mais il n'est pas nécessaire de tout lire tout de suite !)

En plus, sur pas mal de distributions récentes, un démon systemd est
créé lors que la connexion de chaque utilisateur : du coup, les
utilisateurs peuvent mettre en place, à l'aide de ce démon, les
services dont ils souhaitent disposer, sans pour autant affecter tout
le système.

Dans ce TP/tuto, nous vous proposons d'expérimenter, en utilisant
systemd pour installer un petit service (un serveur web totalement
bidon), d'abord en tant qu'utilisateur lambda (pas risqué du tout),
ensuite en tant que super-utilisateur (un peu plus risqué puisque vous
allez sûrement le faire sur votre ordi perso).

Pour que vous puissiez faire ce TP/tuto, il faut donc que vous
disposiez d'une distribution GNU/Linux qui utilise systemd, et qui
lance un systemd à la connexion de chaque utilisateur : avec Ubuntu,
c'est vérifié ; avec Debian ça doit fonctionner, avec Fedora aussi et
Archlinux également. Par contre, les auteurs ne peuvent que croiser
les doigts pour les autres distribs...

Notez aussi que les auteurs de ce TP/tuto ne sont pas, pour de vrai,
des experts de systemd. Voici d'ailleurs les liens qu'ils ont utilisés
pour préparer le sujet, et qui peuvent vous être utiles :
* https://www.linux.com/tutorials/understanding-and-using-systemd/
* https://doc.ubuntu-fr.org/creer_un_service_avec_systemd
* https://access.redhat.com/documentation/fr-fr/red_hat_enterprise_linux/7/html/system_administrators_guide/sect-managing_services_with_systemd-unit_files
* https://wiki.archlinux.org/index.php/Systemd/User

Pour ce TP/tuto, nous vous demandons donc de nous faire un petit
rapport sur :
* ce qui vous a posé problème en essayant de suivre le tuto,
* quelles solutions vous avez tentées,
* d'expérimenter avec au moins un autre service que le service bidon
  utilisé dans le sujet.

## En "mode utilisateur"

### Les répertoires à créer

Créer les répertoires
* `$HOME/bin`
* `$HOME/.config/systemd/user`
dans votre répertoire personnel.

Vous pouvez adapter `$HOME/bin` à votre convenance, mais
`$HOME/.config/systemd/user` est (plus ou moins) obligatoire (voir la
section `UNIT FILE LOAD PATH` de `man systemd.unit`).

Il est possible que, sur votre distribution GNU/Linux, le répertoire
`$HOME/.config/systemd/user` existe déjà : dans ce cas, c'est "tout
bon". Si vous devez effectivement le créer, faites un petit `systemctl
--user daemon-reload` ensuite, pour qu'il soit bien pris en compte
dans votre config systemd par le suite.

### Le script de base

Créez un script `$HOME/bin/mini-web.sh` avec le contenu initial
suivant :
```
#!/bin/sh

nc -l -p 8888 -c <<EOF
HTTP/1.1 200 OK


<html>
        <head>
                <title>Page de test html toute simple :D</title>
        </head>
        <body>
                <h1>A la une : toto fait du vélo !!!</h1>
        </body>
</html>
EOF
```

Vous pouvez bien entendu remplacer `toto fait du vélo !!!` par autre
chose (si vous en avez marre de toto), mais par contre les auteurs de
ces lignes pensent qu'il est important d'avoir 3 retours à la ligne
après la ligne `HTTP/1.1 200 OK` (vous pouvez expérimenter). Notez
aussi :

* l'utilisation du *couteau-suisse TCP-UDP* `nc` ; les auteurs de ces
lignes ne sont pas des experts, mais c'est bien pratique de savoir
jouer un peu avec... À noter qu'il existe plusieurs versions de nc, et
que la version de ce TP est celle d'OpenBSD (typiquement celle
utilisée sous Ubuntu). Si vous utilisez la version de GNU (par exemple
sur Archlinux), la commande sera alors `nc -l -p 8888 -c`

* `nc -l 8888 -q 0` peut être utilisé à la place de `nc -l -p 8888 -c`
sur certaines distributions (il existe plein de version différente de `nc`).

* l'utilisation d'un [*here document*][https://fr.wikipedia.org/wiki/Here_document].

N'oubliez pas de rendre votre script exécutable avec `chmod +x`, puis
lancez votre script, et vérifiez avec votre navigateur préféré que le
script fonctionne (une fois), pour afficher le petit morceau de page
web qu'il contient, en utilisant l'url `localhost:8888`.

### Première tentative de mise en place du service

Créez un fichier `$HOME/.config/systemd/user/mini-web.service`
(fichier texte régulier), avec le contenu suivant. Veillez à adapter
le chemin `/home/nlouvet/bin/mini-web.sh` avec votre répertoire
personnel (pas certain que l'on puisse utiliser la variable
d'environnement `$HOME` ; à essayer éventuellement). Ce fichier est un
"fichier unité" (*unit file* en irlandais), et permet de déclarer un
service auprès de systemd.

```
[Unit]
Description=Page web de test sur le port 8888

[Service]
Type=simple
ExecStart=/home/nlouvet/bin/mini-web.sh # À modifier avec le chemin de votre script

[Install]
WantedBy=default.target
```

Logiquement, ce fichier doit vous permettre de déclarer le service
auprès de votre démon systemd utilisateur (voir
[ici][https://wiki.archlinux.org/index.php/Systemd/User]).

Une fois que vous avez enregistré le fichier `mini-web.service` :
* rechargez votre démon systemd avec `systemctl --user daemon-reload`
  (ça ne peut pas nuire) ;
* lancez le service `mini-web.service` avec `systemctl --user start
  mini-web.service` ;
* consultez l'état de votre service avec `systemctl --user status
  mini-web.service` (touche `q` pour quitter) ; logiquement, vous
  devez avoir confirmation du fait que votre service est actif, et
  obtenir les PID des processus en jeu ;
* vérifiez que votre service apparaît bien dans la liste des services
  utilisateurs chargés avec `systemctl --user list-units
  --type=service` (encore `q` pour quitter).

Ensuite, il est intéressant de consulter la liste des processus de
votre ordi (`ps -ef`), et de vérifier que :
* vous voyez bien le processus `nc` en cours d'exécution ;
* que le père de `nc` est bien votre script `mini-web.sh` ;
* que le père de `mini-web.sh` est bien votre `systemd` utilisateur ;
* que le père de votre `systemd` est bien le `systemd` du système, de PID 1.

Maintenant, retournez dans votre navigateur web préféré, et consultez
`localhost:8888` ; logiquement, la page doit s'afficher. Par contre,
si vous rafraichissez la page, elle disparait : votre script est mort
! Vérifiez cela :
* `systemctl --user status mini-web.service` doit vous dire que le
  service est "dead" (comme Zed : "Zed's dead") ;
* avec `ps -ef`, vous voyez que vos beaux processus ont disparu :c
* `systemctl --user list-units --type=service` ne fait plus apparaître
  votre service.

La conclusion est que, lorsque l'on crée un service simple
(`Type=simple`), et que le script n'est prévu pour s'exécuter qu'une
seule fois, on ne peut utiliser le service qu'une seul fois !

### Deuxième tentative

On va ajouter l'option `Restart=always` et expérimenter. Le fichier
unité ressemble maintenant à ceci :

```
[Unit]
Description=Page web de test sur le port 8888

[Service]
Type=simple
ExecStart=/home/nlouvet/bin/mini-web.sh
Restart=always


[Install]
WantedBy=default.target
```

Après avoir modifié le fichier `mini-web.service` :
* il faut *recharger* le service avec `systemctl --user restart
  mini-web.service` ;
* vérifiez que le service est bien chargé avec `systemctl --user
  status mini-web.service`.

Observez ce qui se passe quand, dans votre navigateur web, vous
chargez l'url `localhost:8888`, et qu'ensuite vous la rechargez
plusieurs fois à un rythme pas trop rapide (1 ou 2 secondes entre
chaque frappe de F5) : logiquement, tout doit bien se passer, la page
se ré-affiche normalement !

Maintenant, tentez de rafraichir "frénétiquement" (disons 10 fois en
moins de 5 secondes) la page web dans votre navigateur : que se
passe-t-il ? Mais pourquoi donc ?

En fait, systemd refuse de relancer trop fréquemment le service : il y
a des limites modifiables, avec l'option `RestartSec` par exemple,
dans la section `[Service]`. C'est normal : on ne peut pas consacrer
toutes les ressources de la machine à un service qui "plante" sans
arrêt (c'est bien ce que fait notre script `mini-web.sh`, car il se
termine après chaque connexion), que ce soit parce qu'il est instable,
ou à cause d'une "attaque"...

Vous pouvez expérimenter avec l'option `RestartSec` (au moins vérifier
que les auteurs de ces lignes ne vous racontent pas d'histoires en
regardant les docs !), mais cela ne semble par être une option
prometteuse pour maintenir dans un état correct notre service. Il vaut
mieux retravailler notre script !

### Cette fois-ci, c'est la bonne !

On modifie notre script de façon à ce qu'il accepte des demandes de
connexions successives. En plus, maintenant il donne l'heure ! Le
voici (oui, c'est mal indenté, mais il ne faut pas toucher aux EOF,
sinon ça ne peut pas fonctionner) :

```
#!/bin/sh

while true; do
d=$(date)
nc -l -p 8888 -c <<EOF
HTTP/1.1 200 OK


<html>
  <head><title>Page de test html toute simple :D</title></head>
    <body>
      <h1>Quelle heure est-il ?</h1>
      <p>Il est $d</p>
    </body>
</html>
EOF
done
```

Il n'est pas nécessaire de conserver la ligne `Restart=always`,
puisque maintenant notre super serveur se remet à l'écoute sur le port
8888 après chaque connexion : vous pouvez l'enlever (pensez bien à
demander à votre systemd de recharger sa configuration, et à relancer
votre service). Testez votre service depuis votre navigateur :
logiquement, tout doit fonctionner !

### On installe le service !

Dans l'état actuel des chose, votre systemd prend en charge le service
`mini-web.service`, mais le service ne sera pas rechargé la prochaine
fois que vous vous reconnecterez sur le système. Vous pouvez le
vérifier par vous-même, mais l'auteur de ces lignes vous conseille de
le croire sur parole !

Pour cela, il faut "installer" (traduction dans le contexte de
*enable*, mais ce n'est pas forcément la meilleure...) :

```
systemctl --user enable mini-web.service
```

Logiquement, `systemctl` doit vous répondre qu'il crée un lien
symbolique qui va bien pour que le service soit relancé à votre la
prochaine connexion.

Pour bien être sûr, vous devez maintenant vous déconnecter de votre session
graphique, voir redémarrer votre ordi, pour vérifier que vous
retrouvez bien le service `mini-web.service` frais et dispo lorsque
vous vous reconnectez !

Une fois que vous avez fini d'expérimenter, vous devez "désinstaller"
(*disable*) le service avec :

```
systemctl --user disable mini-web.service
```

## En "mode super-utilisateur"

Logiquement, à ce stade du jeu, vous devez déjà avoir pas mal compris
"par la pratique". Maintenant, il s'agit d'adapter les manips pour que
notre service soit pris en charge par le démon `systemd` de PID 1 :
ainsi, le service sera disponible dès le démarrage du système (avec
notre processus bidon, ça ne sert à rien, mais c'est le cas pour des
démons plus "sérieux" comme `sshd` ou `httpd`).

Pour cela, vous devez placez dans `/etc/systemd/system` un fichier
unité `mini-web.service` (de propriétaire et groupe `root:root`, et
avec les droits `rw-r--r--`). Ce fichier doit définir une unité qui
correspond au service "miniweb", qui doit relancer votre script
`mini-web.sh` à chaque fois qu'il se termine. Aller, voici un exemple
de fichier :

```
[Unit]
Description=Page web de test sur le port 8888
After=network.target

[Service]
Type=simple
User=nlouvet
ExecStart=/home/nlouvet/bin/mini-web.sh

[Install]
WantedBy=default.target
```
C'est presque la même chose que dans le cas "utilisateur" ;
simplement, on a ajouté la contrainte `After=network.target` pour que
le service ne soit démarré qu'après le réseau. On a aussi ajouté
`User=nlouvet` pour que le service s'exécute pour le compte de
l'utilisateur `nlouvet`.

Pour la suite, vous pouvez utiliser la commande `systemctl` comme
précédemment, mais il faut l'exécuter sur le compte du
super-utilisateur ; il faudra faire par exemple `sudo systemctl
daemon-reload` pour recharger la config (plus de `--user`).



