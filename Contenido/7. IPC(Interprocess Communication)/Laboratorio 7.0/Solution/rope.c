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



//INCLUDE                --> We only use this header.h because all the other libraries we use are inside that one, so we won't have to import them every time.
#include "header.h"

int main(int argc, char *argv[]){
  //VARIABLES
  //Semaphore ID
  int semaphoreId;
  
  //We don't like plotholes.
	printf("Oh-oh! Zoboomafoo called sick today. Call all of our extras now!\n");
  
  //Apparently we use key_t for requesting resources for out semaphores    -->   http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/key.html
  key_t erebor;     //https://lotr.fandom.com/wiki/Key_to_the_Side-door

  //Checks if the key to Erebor is working correctly.          --> Validation taken from PoloGarcia
  if((erebor = ftok("/dev/null", 65)) == (key_t) - 1){               //     --> ftok is used for keys --> http://pubs.opengroup.org/onlinepubs/009696899/functions/ftok.html
    perror("The key to Erebor is a fake and it takes you to nowhere! Go back to Rivendell and sort this out fast.");
    return -1;
  }

  //Now here we use semget and the creation of an IPC to determine if there is an error or not.        -->Validation taken from PoloGarcia with the adding of IPC_CREAT part
  if((semaphoreId = semget(erebor, 4, 0666 | IPC_CREAT)) < 0){
    perror("semget");
    return -1;
  }

  //If everything is peachy, continues with each individual semaphore    --> This ones are based on the DEFINES made in the header.h
  semctl(semaphoreId, ROPE, SETVAL, 1);
  semctl(semaphoreId, MUTEX , SETVAL, 1);        //We use semctl because it is a system call for operating on semaphores  -->https://www.tldp.org/LDP/lpg/node53.html
  semctl(semaphoreId, baboonN, SETVAL, 0);      
  semctl(semaphoreId, baboonS, SETVAL, 0);
  
  //End the program properly, please!
  return 0;
  
}
