/*
  Made by Valter Núñez -- A01206138

  ***
      Based on:
      The Dining Savages code (Little Book of Semaphores)
      Rudy and Valter's Code (Lemurs)
  ***

*/

//INCLUDES
#include "header.h"

//MAIN
int main(int argc, char const *argv[]) {
  //VARIABLES
	int pid, semid;
	key_t erebor;

	if(argc != 1){
		printf("usage: %s\n", argv[0]);
		return -1;
	}

    //Checks if the key to Erebor is working correctly.
  if((erebor = ftok("/dev/null", 65)) == (key_t) - 1){               //     --> ftok is used for keys --> http://pubs.opengroup.org/onlinepubs/009696899/functions/ftok.html
    perror("ftok");
    return -1;
  }

  //Now here we use semget and the creation of an IPC to determine if there is an error or not.        -->Validation taken from PoloGarcia with the adding of IPC_CREAT part
  if((semid = semget(erebor, 4, 0666 | IPC_CREAT)) < 0){
    perror("semget");
    return -1;
  }

  printf("El valor de max en BEAAAAAR es: %i", max);
  
  int ira = semctl(semid, PORTION, GETVAL, 0);

  printf("\nIRA = %i \n", ira);
  while (1){
    printf("\nLotso (%i) is sleeping peacefully.\n", getpid());
    mutex_wait(semid, FULLPOT);
      printf("The tar is full. Lotso (%i) smells all that honey and awakes.\n", getpid());
      printf("Lotso (%i) starts eating.\n", getpid());
      sleep(2);
      aquire(semid, SERVINGS, max);
      release(semid, EMPTYPOT, 1);

      printf("Lotso (%i) is quite full and ready for hibernation. Let's go sleep. \n", getpid());
      sleep(1);
    mutex_signal(semid, MUTEX);
  }

  return 0;
}
