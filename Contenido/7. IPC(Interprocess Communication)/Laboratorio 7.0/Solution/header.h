/*

    Made by Rudy and Valter
    
    
    
    ***
        Based on: 
          The RiverCrossing code (Little book of Semaphores)
          PoloGarcia's Cigarrete-Smokers Code            -->https://github.com/PoloGarcia/Classical-Semaphores/blob/master/Cigarette-Smokers/smokers.c
          BaboonCrossing code (Little book of Semaphores)
          Allen Tabert's Dining Savages Code             -->https://github.com/pythonGuy/classic-threads/blob/master/DiningSavages.c
          Manchas2k4 Santa Code                          -->https://github.com/Manchas2k4/advanced_programming/tree/master/ipc/semaphores/santa                       
          
       Special thanks to:
      		Zoboomafoo for all the laughs and inspiration for this program. #NeverForget
    ***
  
*/

//We use the IFnDEF to prevent double declaration     -->https://stackoverflow.com/questions/1653958/why-are-ifndef-and-define-used-in-c-header-files
#ifndef LEMUR_H
#define LEMUR_H

//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>


//DEFINES
#define ROPE 0
#define MUTEX 1
#define baboonN 2
#define baboonS 3



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