/*
  Valter Núñez
   A01206138

   Bear and bees  --> Dining Savages Variant

  Based on:
    LBoS's Dining Savages --> http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf

*/

//INCLUDES
#include "header.h"

//Main
int main(int argc, char const *argv[]) {
  //VARIABLES
  int semid;     //SemaphoreId
  key_t erebor;  //Key
  int size = 0;
  unsigned short final_values[4];
  int i;

  //Checks if the key to Erebor is working correctly.
  if((erebor = ftok("/dev/null", 65)) == (key_t) - 1){               //--> ftok is used for keys --> http://pubs.opengroup.org/onlinepubs/009696899/functions/ftok.html
    perror("ftok");
    return -1;
  }

  //Now here we use semget and the creation of an IPC to determine if there is an error or not.        -->Validation taken from PoloGarcia with the adding of IPC_CREAT part
  if((semid = semget(erebor, 5, 0666 | IPC_CREAT)) < 0){
    perror("semget");
    return -1;
  }

 	size = atoi(argv[1]);
 	
	if (size < 1) {
		printf("%s: The size must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}

  
  //If everything is peachy, continues with each individual semaphore    --> This ones are based on the DEFINES made in the header.h
  semctl(semid, SERVINGS, SETVAL, 0);
  semctl(semid, MUTEX, SETVAL, 1);
  semctl(semid, EMPTYPOT, SETVAL, 0);        //We use semctl because it is a system call for operating on semaphores  -->https://www.tldp.org/LDP/lpg/node53.html
  semctl(semid, FULLPOT, SETVAL, 0);
  semctl(semid, PORTION, SETVAL, 100);

  semctl(semid, 0, GETALL, final_values);
	printf("values: ");
	for (i = 0; i < 4; i++) {
		printf("%3i", final_values[i]);
	}
	printf("\n");
  
  
  //Return to end
  return 0;
}
