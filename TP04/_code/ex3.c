#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sched.h>
#include <stdlib.h>
#include <signal.h>

/*MIF18 : Lab 5 Linux Scheduling, ex3*/
/* Grégoire Pichon, 2020*/

#define ONE_MILLION 1000000 // pour convertir les secondes en microsecondes

#define DEBUG 1

// une bonne grosse variable globale pour être acsessible au handler du signal 
FILE* mylogfile;


void ferme_proprement(int sig){
  fclose(mylogfile);
  printf("closed with success\n");
  exit(EXIT_SUCCESS);
}


int main(int argc, char** argv){

  unsigned long waiting, elapsed;
  struct timeval tv[2], begin;
  int cur_pos = 0;
  
  if (argc == 1){
    mylogfile=fopen("ex3_logfile.csv","w");
  }
  else {
    mylogfile=fopen(argv[1],"w");
  }
  
  if (mylogfile == NULL){
    printf("Cannot open file\n");
    exit(EXIT_FAILURE);
  }
  else {
    printf("log file successfully opened\n");
    fprintf(mylogfile,"waiting,elapsed\n");
  }

  // Maintenant un petit gestionnaire du signal SIGINT
  // pour terminer proprement.
  // Ce signal sera envoyé via pkill afin de terminer le programme
  struct sigaction sa;
  sigaction(SIGINT, NULL, &sa);
  sa.sa_handler = ferme_proprement;
  int res = sigaction(SIGINT, &sa, NULL);
  if (res==-1) {
    printf("installation du gestionnaire pour SIGKILL error");
    exit(EXIT_FAILURE);
  }
  
  gettimeofday(&tv[1], NULL);
  gettimeofday(&begin, NULL);


  /* begin représente le moment où le processus commence à s'executer */
  /* tv[2] permet de sauvegarder le temps des deux derniers appels à gettimeofday() */
  while(1){
    
    gettimeofday(&tv[cur_pos], NULL);

    //TODO: calculer waiting, le temps entre les deux derniers gettimeofday()

    /* Si le temps entre les deux appels est grand, le processus a été désordonnancé */
    if (waiting > 500){

      //TODO: calculer elapsed, le temps pendant lequel le processus a tourné avant d'être désordonnancé
      
      if (DEBUG){
	printf("I did not run during %8lu microseconds, I was elapsed during %lu microseconds\n", waiting, elapsed);
      }
      fprintf(mylogfile,"%lu,%lu\n", waiting, elapsed);

      //TODO: mettre à jour begin pour sauvegarder quand les calculs ont repris
    }

    /* Changement de position dans le tableau à deux éléments */
    cur_pos = 1-cur_pos;
  }

  if (DEBUG) {
    printf("numbers are stored in the ex3...csv file\n");
  }

  return 0;
}
