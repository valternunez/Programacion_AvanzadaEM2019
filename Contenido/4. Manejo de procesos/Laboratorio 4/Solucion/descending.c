//Valter Núñez - A01206138
//Manejo de procesos


//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


//Function that creates a child process
void child_generator(int current, int level, char *program){
  //Variable Declaration
	int pid;
  int i;

	//Print 'Visual level Tab'
	for (i = 0; i<current; i++){
    printf("   ");
  }
	//Print info
	printf("PPID = %i PID = %i NIVEL = %i\n", getppid(), getpid(), current);

	//Sleep one second son
	if (current == level){
		sleep(1);
		exit(0);
	}

	for (i = 0; i<=current; i++){
		//Error detection
		if ( (pid = fork()) < 0 ){
			perror(program);
			exit(-1);
		}
		//Recursive calling to generate a child process
		else if (pid == 0){
			child_generator(current + 1, level, program);
			break;
		}
		//Prevents zombie processes
		else{
			wait(NULL);
		}
	}
}

//Main function
int main(int argc, char* argv[]){

  //Error in amount of input
	if (argc != 2){
		printf("usage: %s number_of_levels\n", argv[0]);
		return -1;
	}

  //Variable declaration
	char *endp;
	int num = (int)strtol(argv[1], &endp, 10);

  //Error in the type of input
	if (*endp != '\0' || num < 1){
		printf("%s: The parameter must be a positive integer number greater than zero. \n", argv[0]);
		return -1;
	}

  //Send to function to create a child process
	child_generator(0, num, argv[0]);

  //End program
	return 0;
}
