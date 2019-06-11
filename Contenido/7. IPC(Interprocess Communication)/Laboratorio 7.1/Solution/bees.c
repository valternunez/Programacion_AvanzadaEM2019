/*

*/

//INCLUDES
#include "header.h"

//Procedure
void bees(int elmax, const char* program){
  //VARIABLES
  int semid;     //SemaphoreId
  key_t erebor;  //Key

  //Checks if the key to Erebor is working correctly.
  if((erebor = ftok("/dev/null", 65)) == (key_t) - 1){               //     --> ftok is used for keys --> http://pubs.opengroup.org/onlinepubs/009696899/functions/ftok.html
    perror("ftok");
    exit (-1);
  }

  //Now here we use semget and the creation of an IPC to determine if there is an error or not.        -->Validation taken from PoloGarcia with the adding of IPC_CREAT part
  if((semid = semget(erebor, 4, 0666 | IPC_CREAT)) < 0){
    perror("semget");
    exit (-1);
  }

  int shmid;  //Shared Memory
  //Error validation taken from Peter's Santa Code
  if ((shmid = shmget(erebor, 2 * sizeof(int), IPC_CREAT | 0600)) == -1) {
    perror("shmget");
    exit(-1);
  }

  int i, drop, curr;

  printf ("El valor de elmax antes del for = %i\n", elmax);
  srand( getpid() );
  	for (i=0; i < 10; i++) {

      //Random of droppings per bee
  		drop = rand() % 3 + 1;

  		printf("\nBee %i is trying to drop %i kgs of honey. \n", getpid(), drop);
  		mutex_wait(semid, MUTEX);
  		printf("Bee %i checks the tar.\n", getpid());

  		curr = semctl(semid, SERVINGS, GETVAL, 0);

  		printf("The current kgs in the tar are %i.\n", curr);

      elmax = semctl(semid, PORTION, GETVAL, 0);
      
      printf("\nEl valor de ELMAX antes de el IF es %i\n", elmax);
      
  		if (curr == elmax) {

  			printf("Damn, We can't get any more honey in here. We finished! It would be a shame if Lotso woke up.\n");
  			mutex_signal(semid, FULLPOT);
  			mutex_wait(semid, EMPTYPOT);
  		}

  		if ((curr + drop) > elmax) {
  			printf("There is not enough space for all the honey I've got. Guess I'll just put %i kgs.\n", (elmax - curr));
  			release(semid, SERVINGS, (elmax - curr));
  		} else {
  			printf("Great! I can put everything here. Bee %i drops its honey.\n", getpid());
  			release(semid, SERVINGS, drop);
  		}

  		printf("Bee %i goes to collect more honey.\n", getpid());
  		mutex_signal(semid, MUTEX);

  		sleep((rand() % 3) + 1);
    }//END FOR
}//END Function

//MAIN
int main(int argc, char const *argv[]) {
  
  
  printf("El valor de max al empezar bees es %i\n", max);
  //VARIABLES
	int pid;

	if(argc != 2){
		fprintf(stderr, "usage: %s number_of_bees.\n", argv[0] );
		return -1;
	}

	//We hate atoi. Strtol is classier.   -->https://i.kym-cdn.com/photos/images/newsfeed/001/474/962/dec.jpg
	int num;
	char *x;

	num = (int)strtol(argv[1], &x, 10);

	//Come on. Obviously there are not negative numbers allowed.
	if(num < 1){
		printf("%s: You can't have negative bees, bro.\n", argv[0]);
		return -1;
	}

  int i;
  
  //Run the bee procedure depending on number of bees
  for (i=0; i < num; i++){
    if ((pid = fork() < 0)){
      perror("fork");
      return -1;
    } else if (pid == 0){
      bees(max, argv[0]);
    }
  }

  //End
  return 0;
}
