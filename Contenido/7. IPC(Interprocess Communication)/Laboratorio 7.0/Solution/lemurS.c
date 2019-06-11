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

//INCLUDE   --> We only use this header.h because all the other libraries we use are inside that one, so we won't have to import them every time.
#include "header.h"

//Procedure to move baboons from South to North
void baboon_SN(char* program){
	
	//VARIABLES
	int semaphoreId;
	key_t erebor; //https://lotr.fandom.com/wiki/Key_to_the_Side-door

	//Checks if the key to Erebor is working correctly.          --> Validation taken from PoloGarcia
	if((erebor = ftok("/dev/null", 65)) == (key_t) -1){
		perror("The key to Erebor is a fake and it takes you to nowhere! Go back to Rivendell and sort this out fast.");
		exit(-1);
	}

	//Now here we use semget and the creation of an IPC to determine if there is an error or not.   Checks if the key to Erebor is working correctly.          --> Validation taken from PoloGarcia
	if((semaphoreId = semget(erebor, 4, 0666))  < 0){
		perror("The semaphore linked to the Key to Erebor has a problem. Should you continue? Stop? Continue with caution? No idea. Let's try again!");
		exit(-1);
	}

	int shmid;
	//Error validation taken from Peter's Santa Code
	if ((shmid = shmget(erebor, 2 * sizeof(int), IPC_CREAT | 0600)) == -1) {
		perror("SHHH! I believe the Lemurs are sleeping. You got an error, though. Shmget or something. ");
		exit(-1);
	}
	
	//Let The Baboons Loose, matey
	
	//Show the id of the created North process
	printf("South Zoboomafoo with id %i is trying to cross the rope to get to the show.\n", getpid());
	
	//We use mutex wait because it ensures that the concurrent programming is done correctly.      -->https://mortoray.com/2010/11/18/cpu-memory-why-do-i-need-a-mutex/
	mutex_wait(semaphoreId, MUTEX);
	//We get the if of the process trying to 'cross'. Also we get the space available on the rope semaphore with semctl.       --> We used the idea behind Dining Savages for this from Allen Talbert, using the semctl, as counseled by Juan Arturo Cruz 
	printf("South Zoboomafoo with id %i is trying to cross the rope but there is only %i space available. Can he do it?\n", getpid(), semctl(semaphoreId, ROPE, GETVAL, 0));
	//Lets call the aquire function from header.h. It is the wait.
	aquire(semaphoreId, ROPE, 1);
	
	//We get the value from the rope semaphore. If it is full, we gotta quit some lemurs from the rope.
	if(semctl(semaphoreId, baboonN, GETVAL, 0) == 1){
		printf("Oh, crap! The rope is full. The show will be delayed...\n");
    	//Let's call the release function from header. It is the signal.
    	release(semaphoreId, ROPE, 1);
	}
	
	//If the rope is not full, lets continue :)
	else{
		
  		//Let's call the release function from header. It is the signal.
    	release(semaphoreId, baboonS, 1);
    	//We get the id from the lemur that wants to cross.
  		printf("South Zoboomafoo with id %i gets in the rope. He is trying to get to the show!\n", getpid());
  		//We show how many north and south lemurs are in the rope at the moment.
    	printf("%i North Zoboomafoo(s) are in the rope, %i South Zoboomafoo(s) are in the rope. Some are definetly going in the wrong direction! Silly lemurs\n", semctl(semaphoreId, baboonN, GETVAL, 0), semctl(semaphoreId, baboonS, GETVAL, 0));
	}
	
	//If the ROPE has 0 space available, we gotta wait 1 second for it to 'clean'.
	if(semctl(semaphoreId, ROPE, GETVAL, 0) == 0){
		
		//Let's call the release function from header. It is the signal.
    	release(semaphoreId, ROPE, 4);
    	printf("There are %i North Zoboomafoo(s) crossing right now. Also, there are %i South Zoboomafoo(s) crossing. This is getting confusing. ANIMAL EMERGENCY!\n\n", semctl(semaphoreId, baboonN, GETVAL, 0), semctl(semaphoreId, baboonS, GETVAL, 0));
		sleep(1);  //Let's all calm down a second. (Literally)
    	semctl(semaphoreId, baboonN, SETVAL, 0); //We restart this counter 
    	semctl(semaphoreId, baboonS , SETVAL, 0);  //We restart this counter
	}
	
	//We just signal using the mutex for concurrent programming.
	mutex_signal(semaphoreId, MUTEX);

	//Let's gooo. All good, though.
	exit(0);
}

//Main Function
int main(int argc, char* argv[]){
	//Variables
	int pid;

	//If you miss writing the amount of Lemurs you need, you will need to start again. SorryNotSorry
	if(argc != 2){
		printf("PRODUCER NOTE: Kratt Brothers, remember that you need to ask for the exact amount of Southener Zoboomafoos you want after the program name.\n");
		return -1;
	}

	//We hate atoi. Strtol is classier.   -->https://i.kym-cdn.com/photos/images/newsfeed/001/474/962/dec.jpg
	int num;
	char *x;
	
	num = (int)strtol(argv[1], &x, 10);
	
	//Come on. Obviously there are not negative numbers allowed. 
	if(num < 1){
		printf("%s: Oh God, you just desintegrated some Zoboomafoos. Thanos would be proud, you monster!\n", argv[0]);
		return -1;
	}
	
	//Basic counter for 'for'. We shouldn't tell you this. 
	int i;
	
	//We iterate in the amount of South Lemurs needed
	for(i = 0; i < num; i++){
		
		//If for some unkown reason we get a Parent id negative, we shall call it of :(
		if((pid = fork()) < 0){
			perror("Oh wow. Who would've thought a lemur could stab himself with a FORK.");
			return -1;
		} 
		
		if(pid == 0){    //Child Process
			baboon_SN(argv[0]);   //Let the southener party begin. Unbowed, Unbent, Unbroken or something like that. #OberynLives
		} 
	}
	
	//Please end the program properly. We are not children anymore, alright?
	return 0;
}
