# MIF18-Lab TP4 : Ordonnancement Linux

  * Grégoire Pichon, Université Lyon 1, LIP [email](mailto:gregoire.pichon@univ-lyon1.fr)
  * Version: 2021

## Rendu du TP

* Livrer le code des exercices, un Makefile, dans une unique archive sous Linux et la déposer sur TOMUSS.
* Deadline stricte le jeudi 06/05 à 23h59.

## Description du TP

* L'objectif de ce TP est d'observer les comportements d'un système de
  type UNIX pour l'ordonnancement des processus. Il s'agit ici de
  comprendre les différents comportements du système et d'effectuer
  des expériences sur la durée d'exécution de programme en fonction de
  leur priorité.

* Pour ce TP, vous aurez besoin de travailler sur un système Linux. Si
  vous n'avez pas de système de ce type installé sur votre machine,
  vous pouvez tenter d'utiliser une machine virtuelle ou WSL, sans
  garantie de la part de l'auteur de ce sujet. La solution la plus
  consistante est de se connecter à une machine distante de Lyon 1.

* Les comportements observés dans ce TP ne sont pas forcément
  reproductibles d'un système à un autre, selon la version du noyau
  Linux par exemple. Il sera intéressant de noter vos configurations, donc.

* Lors de vos expériences, essayez d'avoir peu de programmes
  "gourmands" qui tournent sur votre machine, afin que les résultats
  soient représentatifs et pas pollués par des applications qui
  utilisent une grande partie du temps processeur.

## Outils utiles pour ce TP

* Pour connaître le nombre de processeurs disponibles sur votre
  système, vous pouvez regarder le fichier `/proc/cpuinfo`.

* Pour afficher les processus en cours, vous pouvez utiliser la
  commande `top` ou bien `htop` qui a l'avantage d'afficher les
  processus par unité de calcul.

* Pour verrouiller (binder) un processus sur un seul processeur, vous
  pouvez utiliser la commande `taskset`. Par exemple pour binder votre
  programme sur le processeur d'indice 0:
```C
taskset -c 0 ./mon_programme
```

* Pour changer la priorité d'un processus, vous pouvez utiliser la
  commande `nice` ou `renice`. Par exemple pour binder votre programme
  sur le processeur d'indice 0 avec la priorité 5:
```C
taskset -c 0 nice -n 5 ./mon_programme
```
Attention, sur certains systèmes Linux, les valeurs de nice ne sont
pas prises en compte entre différentes session (une session correspond
grosso modo à un terminal). Deux processus dans un même terminal
(lancés en tâche de fond) respecteront bien les priorités mais pas
deux processus lancés dans deux terminaux différents. Sur un Ubuntu
LTS 18.04 LTS le problème n'apparaît pas. Sur un Ubuntu 19.10 il faut
bien lancer les executables depuis le même terminal pour que les
priorités fonctionnent.

<!-- * Pour tracer des courbes, écrire dans un fichier puis utiliser
  [gnuplot](http://www.gnuplot.info/), ou
  [matplotlib](https://matplotlib.org/tutorials/introductory/pyplot.html)
  avec `python`, comme vous voulez, pourvu que ce soit automatisable
  (et automatisé) -->
  



## Step 1: premières observations

* Écrivez dans `ex1.c` un programme qui fait une attente active infinie avec
  `while(1)`.

* Que pouvez-vous observer dans `top` quand vous lancez plusieurs fois
  ce programme (sur des terminaux différents) ?

* Si vous bindez les différentes instances du programme sur un seul
  coeur, comment se répartit le temps processeur ?

* Jouez maintenant avec la priorité en ayant plusieurs instances de
  votre programme avec des priorités différentes. Qu'observez-vous ?

## Step 2: mesure des changements de contexte

* Le fichier `ex2.c` contient un code qui permet de mesurer le temps
  passé dans une boucle.

* Écrivez un programme qui effectue `N=100 000` appels à
  `sched_yield()` puis affiche le temps requis pour la somme de ces
  appels et recommence.

* Si vous lancez deux instances du programme (sur un même processeur),
  que signifie le temps affiché? Quel est son sens avec une seule
  instance du programme ?


* Complétez un tableau avec le temps entre deux `sched_yield()` avec
  1, 2, 3, 4 et 5 instances du programme. Prenez bien garde de prendre
  un temps cohérent uniquement une fois que toutes les instances du
  programme ont bien été lancées. Vous pouvez par exemple les lancer
  en tâche de fond et prendre le temps après 10 tours de boucle qui
  fait les `N` appels à `sched_yield()`. Commentez. Quel est le temps
  d'un changement de contexte avec une seule instance du programme ?
  Avec plusieurs instances? Faites un dessin pour expliquer ce qu'il
  se passe avec trois instances du programme (un dessin à la main avec
  une photo dans le rapport sera suffisant).


* Comment le coût d'un changement de contexte va t-il jouer sur les
  performances en fonction de la _timeslice_, la durée durant laquelle
  le processus tourne sans être interrompu par un autre processus ?


## Step 3: mesure des timeslices

* Écrivez un programme `ex3.c.` qui fait des `gettimeofday()` et qui
  affiche un message lorsque la durée entre deux appels est
  inhabituellement longue. Cela signifiera alors que le programme a
  été désordonnancé. Le message devra contenir cette durée afin
  d'avoir une estimation de la _timeslice_. 

* Afin de limiter le bruit (issu des autres processus qui s'exécutent
  sur votre machine), vous n'afficherez le message que lorsque la
  durée entre deux appels sera suffisante (par exemple 0.5 ms ou 1
  ms).

* Expliquez le comportement observé quand vous lancez une ou deux
  instances de ce programme (toujours sur le même processeur).

* Tracez un graphe montrant l'évolution de la _timeslice_ en fonction
  de la priorité. Avec deux instances du programme, vous ne jouerez
  qu'avec la priorité d'une des instances. Vous livrerez le code vous
  permettant de faire ce graphe en complétant le programme python
  `analyze_ex3.py`. Le code fourni permet via la fonction
  `acquire_results()` de lancer deux instances du programme dont une
  avec une priorité variable (de 0 à 19). Après une attente de 20
  secondes, les deux processus sont tués. Il vous est demandé de
  compléter ce code afin de parser les fichiers générés par votre
  executable `ex3`. Le code pour afficher les courbes est également
  fourni mais vous pouvez bien évidemment le modifier. Vous utiliserez
  `python3 analyze_ex3.py` pour lancer les expériences (cela prendra
  du temps) et `python3 analyze_ex3.py parse` pour parser et afficher
  les données.
  
* Qu'en concluez vous ?


## Step 4: thread versus processus

* Mêmes questions pour un processus multi-threadé. Par rapport à un
  processus mono-thread, comment le temps CPU est-il réparti ? Mesurez
  les changements de contexte et concluez sur les surcoûts de
  l'ordonnancement.

