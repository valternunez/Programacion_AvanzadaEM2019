/*

  Made by Valter Núñez -- A01206138

  ***
      Based on:
        The Dining Savages code (Little Book of Semaphores)
        Rudy and Valter's Code (Lemurs)
  ***
*/


#ifndef BEAR_H
#define BEAR_H

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

//DEFINES               --> Semaphores done according to the hint in the LittleBookOfSemaphores
#define SERVINGS 0
#define MUTEX 1
#define EMPTYPOT 2
#define FULLPOT 3
#define PORTION 4


int max;



//Acts as Aquire   --> Taken from renos.c code from Peter
int aquire(int semid, int sem, int val){
  struct sembuf operacion;
  operacion.sem_num = sem;
  operacion.sem_op = -val;
  operacion.sem_flg = 0;

  //Return
  return semop(semid, &operacion, 1);
}

//Acts as Release   --> Taken from renos.c code from Peter
int release(int semid, int sem, int val){

  struct sembuf operacion;
  operacion.sem_num = sem;
  operacion.sem_op = val;
  operacion.sem_flg = 0;

  //Return
  return semop(semid, &operacion, 1);
}


//Just wait function for mutex
int mutex_wait(int semid, int sem){

  //Return
  return aquire(semid, sem, 1);
}


//Literally signal for mutex...
int mutex_signal(int semid, int sem){

  //Return
  return release(semid, sem, 1);
}

#endif
