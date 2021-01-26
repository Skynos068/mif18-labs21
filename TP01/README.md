# MIF18-Lab TP1: Entrées / Sorties

  * Grégoire Pichon, Université Lyon 1, LIP [email](mailto:gregoire.pichon@univ-lyon1.fr)
  * Nicolas Louvet, pareil, sauf l'adresse :D
  * Version: 2020


## Description du problème

Ce TP a pour objectif de sensibiliser à la gestion des entrées/sorties. Dans un premier temps, nous allons nous intéresser à des mécanismes de bufférisation, avant de voir des mécanismes non bloquants. Dans l'ensemble du TP, nous proposons d'utiliser des outils UNIX pour observer les performances induites par les différents mécanismes d'entrées/sorties. L'objectif de ce TP est de mettre en exergue des "bons" comportements à avoir lors de la gestion des entrées/sorties.

## Step 1 : outils Unix pour observer les entrées/sorties

Dans l'ensemble du TP, nous allons utiliser différents outils UNIX pour observer les entrées/sorties, notamment au niveau des performances.

* `strace` - Trace system calls and signals

* `ltrace` - A library call tracer

* `iotop` - Simple top-like I/O monitor

* `iostat` - Report Central Processing Unit (CPU) statistics and input/output statistics for devices and partitions (see also `mpstat` and `vmstat`)

* `perf` - Performance analysis tools for Linux

## Step 2 : entrées/sorties classiques

### Step 2 a : entrées/sorties non bufferisées

Écrivez un code C qui écrit octet par octet dans un fichier `test.txt` avec la primitive `write` ; votre programme doit prendre en paramètre le nombre d'octets à écrire. Pour cet exercice, vous pouvez très bien écrire toujours le même octet (caractère). Dans la suite, on appelle `test1.c` le code source, et `test1` l'exécutable.

### Step 2 b : entrées/sorties bufferisées

Écrivez un nouveau code C, qui fait la même chose que le code de la question précédente, mais en utilisant maintenant la fonction `fwrite` à la place de la primitive `write`. On appelle `test2.c` le code source, et `test2` l'exécutable. Pour un même nombre d'octets à écrire sur le disque, comparez les temps d'exécutions de `test1` et de `test2` : qu'observez vous (logiquement, avec un ordre d'octets de l'ordre de 1Mio, vous devez déjà pouvoir constater des différences entre les deux codes) ?
Comparez également les deux programmes en utilisant `strace` (avec l'option `-c` par exemple) : comment expliquer la différence entre les temps d'exécution des deux
programmes ?

### Step 2 c : performance des entrées/sorties bufferisées "à la main"

Modifiez le code initial (`test1.c`) pour stocker les données à écrire dans un buffer de taille fixe (donnée en paramètre de votre programme), avant
écriture sur le disque avec `write`. On appelle `test3.c` le code source, et `test3` l'exécutable.

Avec un script shell, lancez des expériences pour trouver une taille de buffer permettant d'obtenir de bonnes performances. Pour cela, faites un fichier avec sur chaque ligne : la taille du buffer et le temps d'exécution. Utilisez `gnuplot` (ou d'autres outils comme `Python` ou `R` si vous les maîtrisez mieux) pour afficher les performances obtenues en fonction de la taille du buffer.

## Step 3 : entrées/sorties non bloquantes

Nous allons maintenant voir comment utiliser des entrées/sorties non bloquantes : ce qu'elles permettent de faire, quels en sont les avantages et les inconvénients.

Pour cela, vous allez écrire deux programmes :

* `read_pipe.c` qui lit, octet par octet, sur un tube nommé, dont le nom est `fifo.fif`. Ce programme lit "lentement" un caractère sur le tube toutes les millisecondes.

* `write_pipe.c` qui écrit octet par octet sur le tube `fifo.fif`. Le programme écrit "aussi vite que possible" : du coup, comme le lecteur est "lent", le tube fini par être rempli, et l'écrivain doit attendre que le lecteur ait fait de la place avant de reprendre.

On appelle `read_pipe` et `write_pipe` les exécutables correspondants. Pour voir ce qui se passe, il est commode que `write_pipe` n'écrive pas toujours la même chose dans le tube : écrivez les lettres de l'alphabet par exemple ; écrivez aussi sur la sortie standard ce que vous écrivez ou ce que vous lisez, de manière à comprendre ce qui se passe. Avant de tester ces programmes, vous devez créer le tube nommé `fifo.fif` avec la commande `mkfifo`.

Commencez par expérimenter avec des appels à `write` bloquants : que fait `write_pipe` quand le tube est plein ? Modifiez ensuite votre code `write_pipe.c` en utilisant la fonction `fcntl` pour positionner le flag `O_NONBLOCK` pour l'écriture. Expérimentez à nouveau, et modifiez `write_pipe.c` de façon à ce qu'il affiche "le tube est plein" quand il constate qu'il ne peut plus écrire (idéalement, une seule fois).

Comparez le cas "écritures bloquantes" avec celui "écritures non bloquantes" : quel est l'intérêt de chacuns des deux cas ?

