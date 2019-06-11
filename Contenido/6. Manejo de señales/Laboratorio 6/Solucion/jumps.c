//Valter Núñez -- A01206138
//Manejo de señales


//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>


//Signal Management Procedure
void handler(int signal) {
    //Signal Options
	switch (signal){
		case SIGHUP:
			printf("Punto de retorno 1\n");
			printf("Tabla de cuadrados\n");
			printf("Número Cuadrado\n");
			printf("1		1\n");
			printf("2		4\n");
			printf("3		9\n");
			printf("4		16\n");
			printf("5		25\n\n");
		case SIGINT:
			printf("Punto de retorno 2:\n");
			printf("Tabla de raíces cuadradas\n");
			printf("Número Raíz cuadrada\n");
			printf("1		1.00000\n");
			printf("2		1.41421\n");
			printf("3		1.73205\n");
			printf("4		2.00000\n");
			printf("5		2.23607\n\n");
		case SIGQUIT:
			printf("Punto de retorno 3:\n");
			printf("Tabla de logaritmos\n");
			printf("Número Logaritmo\n");
			printf("1		0.00000\n");
			printf("2		0.69315\n");
			printf("3		1.09861\n");
			printf("4		1.38629\n");
			printf("5		1.60944\n\n\n");
			break;
		default:
			printf("ERROR in recieved signal. \n");
			break;
	}
}

//Main Function
int main(int argc, char* argv[]) {
	
	//Signals
	signal(SIGHUP, handler);
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);

    //Error detection in arguments
	if (argc != 1) {
		printf("usage: %s \n", argv[0]);
		return -1;
	}
	//If the amount of arguments is correct, do the tables
	else {
		printf("Punto de retorno 1: \n");
		printf("Tabla de cuadrados: \n");
		printf("Numero Cuadrado: \n");
		printf("1		1\n");
		printf("2		4\n");
		printf("3		9\n");
		printf("4		16\n");
		printf("5		25\n");
		printf("Punto de retorno 2: \n");
		printf("Tabla de raices cuadradas: \n");
		printf("Numero Raiz cuadrada: \n");
		printf("1		1.00000\n");
		printf("2		1.41421\n");
		printf("3		1.73205\n");
		printf("4		2.00000\n");
		printf("5		2.23607\n");
		printf("Punto de retorno 3:\n");
		printf("Tabla de logaritmos: \n");
		printf("Numero Logaritmo: \n");
		printf("1		0.00000\n");
		printf("2		0.69315\n");
		printf("3		1.09861\n");
		printf("4		1.38629\n");
		printf("5		1.60944\n");
		printf("PID = %i . \n", getpid());
	}

    //Sleep
	while(1){
		sleep(10);
	}
	
	//END program
	return 0;
	
	//NOTE:
	    //The 'kill' command has to be done in another terminal for it to work.
}