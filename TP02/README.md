# MIF18-Lab TP2: Mémoire virtuelle

  * Grégoire Pichon, Université Lyon 1, LIP [email](mailto:gregoire.pichon@univ-lyon1.fr)
  * Version: 2020

## Rendu du TP

Nous vous demandons de consacrer 3h à ce TP, comme cela était prévu dans l'emploi du temps initial.

Il faudra rendre au plus tard le vendredi 20/03 à minuit et sur TOMUSS une archive tgz contenant 1 page de réponses aux questions et un fichier en C avec les différents morceaux de code demandés.

L'objectif est de montrer que vous avez fait le TP, sans forcément répondre à toutes les questions ni aller au bout si les 3h ne vous suffisent pas.

## Description du TP

L'objectif de ce TP est d'observer le comportement mémoire de
différents programmes. Nous commencerons par observer les défauts de
page apparaissant lors de la lecture / écriture d'un gros fichier. Par
la suite, nous observerons le comportement de l'OS et l'organisation
de l'espace virtuel des processus. Nous verrons notamment comment le
système fait de l'allocation paresseuse et la gestion de la pile et du
tas. Pour finir, des mappings permettront de mettre en place des zones
de partage entre processus.

## Step 1: Défauts de page lors de la lecture d'un fichier

* Utilisez l'outil ``/usr/bin/time`` pour savoir où un programme
faisant des ``sched_yield`` passe du temps.  Expliquez les différents
temps affichés (on pourra comparer avec les appel-systèmes
``gettimeofday`` et ``getpid``).

* Remplacez maintenant votre programme par un programme qui fait des
``sleep`` et expliquez les temps mesurés par ``/usr/bin/time``.

* On va maintenant lire un fichier présent sur le disque mais pas
encore en mémoire. On commence par créer le fichier avec

``
 dd if=/dev/zero of=fichier count=100000 bs=1000
``

* Lisez le fichier avec

``
 /usr/bin/time cat fichier > /dev/null
``

Expliquez les temps observés lors du premier lancement.  Rappelez ce
qu'est ``/dev/null`` et pourquoi on a redirigé la sortie dedans.
Relancez plusieurs fois cette commande et expliquez ce que vous
observez.  Comment le comportement change-t-il si on écrit dans un
vrai fichier ?

## Step 2: Organisation de l'espace virtuel des processus

### Step 2a: Quelques observations

* Déduire de ``/proc/self/maps`` comment les différentes régions
mémoire des processus sont organisées en mémoire virtuelle sous Linux.
Pourquoi la pile et le tas sont-ils placés ainsi ?

* Où est située la mémoire du noyau ? Pourquoi fait-elle partie de
  l'espace virtuel utilisateur du processus alors qu'il n'y a pas
  accès ?

* Quelles variations observe-t-on d'un processus à l'autre, même s'ils
sont lancés exactement de la même façon ?  Pourquoi est-il utile de
faire varier le mapping en espace utilisateur ?  Pourquoi est-il utile
de ne pas faire varier le mapping noyau ?

Afin de relancer un même programme plusieurs fois (cat par exemple),
on pourra utiliser ``watch -n 1``, qui le relancera toutes les
secondes.

Pour la suite de cette partie, on pourra utiliser les lignes suivantes afin d'afficher les VMA du processus courant:

```C
  char s[64];
  sprintf(s, "cat /proc/%u/maps", getpid());
  system(s);
```

### Step 2b: Mémoire du tas

* Écrire un programme qui fait plein de petits ``malloc`` (quelques
kilooctets ou dizaines de kilooctets) et expliquer l'évolution du (ou
des) tas.  Faire maintenant des gros ``malloc`` (de l'ordre du
mégaoctet) et expliquer également. En déduire le comportement du
système vis-à-vis d'un éventuel débordement du tas.

### Step 2c: Mémoire de la pile

* Écrire un programme récursif qui va faire déborder la pile. Pour
savoir à quel moment il a planté, on pourra lui faire afficher
``/proc/self/maps`` régulièrement.

* On pourra également essayer de rattraper le signal SIGSEGV pour
afficher l'adresse exacte où le débordement a eu lieu (cela
nécessitera probablement ``sigaltstack``). Cette partie étant plus
complexe à mettre en place, revenez dessus à la fin du TP si vous avez
le temps uniquement.

Pourquoi le système ne peut-il pas gérer les débordements de pile comme ceux du tas ?

### Step 2d: Mapping en mémoire

* Faire quelques ``mmap`` privés et partagés et retrouver les entrées correspondantes
dans ``/proc/self/maps``.

* Déterminez la taille maximale que l'on peut projeter en mémoire virtuelle d'un
seul coup avec ``mmap``. On utilisera les flags ``MAP_ANONYMOUS|MAP_PRIVATE`` et
le descripteur ``-1`` pour obtenir une projection anonyme (sans fichier associé).
Essayez à nouveau avec ``MAP_NORESERVE`` et expliquez.

* Faites une boucle de gros ``mmap`` jusqu'à ce que le système refuse
de projeter de nouvelles zones. Déduisez-en une approximation de la
taille maximale de l'espace d'adressage du processus.  Comparez cette
taille à la notion d'architecture 64bits, notamment en regardant le
fichier ``/proc/cpuinfo``.

## Step 3: Manipulation de la mémoire virtuelle

### Step 3a: Mappings partagés d'un même fichier

* Créez un fichier de 8ko.

* Écrivez un programme qui projette (``mmap``) trois fois ce fichier dans un même
processus de manière publique (``MAP_SHARED``).

* Écrivez 1111 au début d'un mapping,
puis 2222 dans second mapping (décalé de 4 octets),

* Observez le résultat depuis le troisième mapping, en affichant les 8 premiers octets.

### Step 3b: Copy-On-Write
On utilise maintenant un mapping public et un mapping privé vers le même fichier.

* Écrivez 1111 au début du premier mapping puis 2222 dans le second mapping (avec un décalage de 4 donc)

* Écrivez le contraire dans la deuxième page (2222 décalé de 4096+4 dans le second mapping,
puis 1111 décalé de 4096 dans le premier).

En réfléchissant à l'implémentation du copy-on-write, expliquez le phénomène.
Que se passerait-il si on écrivait à cheval sur les deux pages ?
Comment pourrait-on utiliser ce phénomène pour mesurer la taille des pages ?

### Step 3c: Mesures des défauts de page
Modifiez votre programme pour ne créer qu'un seul mapping public de 1Mo
(et aucun privé).
Ajoutez un argument en ligne de commande pour terminer après une des
étapes intermédiaires:

* avant le mmap,
* après le mmap,
* après une lecture d'un octet, d'une page, ou de tout le mapping,
* après une écriture d'un octet, d'une page, ou de tout le mapping,
* jusqu'à la fin du programme.

Utilisez ``/usr/bin/time`` pour mesurer les défauts de pages de
votre processus selon l'étape à laquelle vous vous arrêtez.
Expliquez les chiffres.

