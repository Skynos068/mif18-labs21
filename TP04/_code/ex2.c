#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include <sched.h>

/*MIF18 : Lab 5 Linux Scheduling, ex2*/
/* Grégoire Pichon, 2020*/ 

#define NB_CALLS 100000
#define ONE_MILLION 1000000 // pour convertir les secondes en microsecondes

int main(){

  while(1){

    unsigned long elapsedMicroseconds;
    struct timeval startTime, endTime;
    
    gettimeofday(&startTime, NULL);

    // TODO : boucle infinie qui effectue NB_CALLS appels à sched_yield
    // (3 lignes)
    
    gettimeofday(&endTime, NULL);

    elapsedMicroseconds = (endTime.tv_sec - startTime.tv_sec) * ONE_MILLION + (endTime.tv_usec - startTime.tv_usec);
    printf("Calling 100 000 sched_yield() took %8lu microseconds\n", elapsedMicroseconds);
  }
  
  return 0;
}
