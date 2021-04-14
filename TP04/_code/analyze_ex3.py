
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
MIF18 TP5 Linux scheduling
Script pour tracer les courbes de l'exo 3. 
Grégoire Pichon et Laure Gonnord, Avril 2020
usage python3 test_ex3.py
"""

import sys
import os,csv
from os import path
import matplotlib.pyplot as plt
import numpy as np
from sys import argv

# experiment config

nb_prios = 19
execundertest="./ex3.exe"
finalfig="ex3_fig.pdf"


if not path.isfile(execundertest):
    print("please compile before")
    exit(1)


def acquire_results(nbp):
    for i in range(0, nbp):
        print("Acquisition pour prio = ",str(i))
        ## Lancer les programmes en tâche de fond
        os.system("taskset -c 0 nice -n 0          ./ex3.exe"+ "> /dev/null &")
        os.system("taskset -c 0 nice -n "+str(i)+" ./ex3.exe "+"output_"+str(i)+".csv > /dev/null &")
        
        # Attendre puis les tuer
        os.system("sleep 20")
        os.system("pkill -INT ex3.exe")

    print("log files generated")
    return


def parse_results(nbp):
    # SD pour stocker 
    X=np.zeros([nbp, 1])
    Y1=np.zeros([nbp, 1])
    Y2=np.zeros([nbp, 1])

    for i in range (0, nbp):
        # Parser les resultats: on recupere les 20 premières lignes (sauf la premiere), au plus.
        # à la fin X[i] <-i, Y1[i] <- moyenne de waiting et Y2[i] <- moyenne elapsed
        # HINT : utiliser csv.DictReader
    return (X, Y1, Y2)

def plot(X, Y1, Y2):
    
# plot config
    font = {'family' : 'normal',
            'weight' : 'bold',
            'size'   : 10}
    plt.rc('font', **font)

    plt.title("One process with priority set to 0, the other with varying priority")
    plt.xlabel("Priority")
    plt.ylabel("Time (ms)")
    plt.yscale("log")
    plt.plot(X, Y1, 'o', label='Waiting time', color='green')
    plt.plot(X, Y1, color='green')
    plt.plot(X, Y2, 'd', label='Running time', color='red')
    plt.plot(X, Y2, color='red')
    plt.legend()
    plt.savefig(finalfig)
    return


if __name__ == '__main__':
    compute = True
    parse = False
    if len(argv) == 2:
        if argv[1] == 'clean':
            if os.path.isfile(finalfig):
                remove('finalfig')
        elif argv[1] == "lazy":
            compute = False
        elif argv[1] == "parse":
            compute = False
            parse = True
    if compute:
        print("Acquisition des données (may take time)")
        acquire_results(nb_prios) # avec moins de prios pour tester!
    if parse:
        print("parse outputfiles")
        (X, Y1, Y2) = parse_results(nb_prios)
        print (X,Y1,Y2)
        print("plot!")
        plot(X, Y1, Y2)
