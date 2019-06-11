/*
    Valter Núñez
    A01206138
    
    EXAMEN 2 -- IPC y Procesos
    
    
    ||| Based on code by Pedro Pérez in 'Tema 8.pdf' Pages 5-6 |||
*/


//INCLUDES
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

//Max size string
#define MAX 256

//Clean string adding the NULL value
void limpia(char *cadena) {
    //Variable declaration
	char *p = strchr(cadena, '\n');
	
	//Add NULL value
	if (p) {
		*p = '\0';
	}
}

//Change characters to CAPS by using ASCII table
void to_caps(char s[]) {
    //Variable declaration
   int i = 0;
   
   //Convert each character to its caps similar with ASCII table  --   https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.ioaq100/ascii_table.gif
   while (s[i] != '\0') {
      if (s[i] >= 'a' && s[i] <= 'z') {
         s[i] = s[i] - 32;
      }
      i++;
   }
   
}


//Main function
int main(int argc, char *argv[]) {
    
    //Variable declaration
	int tuberia_em_re[2];
	int tuberia_re_em[2];
	int pid;
	char mensaje[MAX];
	
	//Amount of arguments error detection
	if (argc != 1) {
	    fprintf(stderr, "usage: %s  string\n", argv[0]);
	    return -1;
	}
	
	//Pipe error detection
	if (pipe(tuberia_em_re) < 0 || pipe(tuberia_re_em) < 0) {
		perror("pipe");
		return -1;
	}
	
	//Fork on process error detection
	if ((pid = fork()) < 0) {
		perror("fork");
		return -1;
	} 
	
	//If everything is OK
	else if (pid == 0) {
	    
	    //Initial Prints
		fprintf(stdout, "Pid = %i parent ...\n", getppid());
		fprintf(stdout, "Pid = %i. PPid = %i process starting ... \n", getpid(), getppid());
		
		//Convert to CAPS
		while(read(tuberia_em_re[0], mensaje, MAX) > 0 && strcmp(mensaje, "EOF") != 0){
			fprintf(stdout, "Pid %i - Read:   %s \n", getpid(), mensaje);
			to_caps(mensaje);
			fprintf(stdout, "%s\n", mensaje);
			strcpy(mensaje, "LISTO");
			write(tuberia_re_em[1], mensaje, strlen(mensaje) + 1);
		}
		
		//Close Pipes 
		close (tuberia_em_re[0]);
		close (tuberia_em_re[1]);
		close (tuberia_re_em[0]);
		close (tuberia_re_em[1]);
		
		//END
		return 0;
	} 
	
	else {
		do {
			fgets(mensaje, MAX, stdin);
			limpia(mensaje);
			write(tuberia_em_re[1], mensaje, strlen(mensaje) + 1);
			if (strcmp(mensaje, "FIN") == 0) {
				break;
			}
			do {

				strcpy(mensaje, "");
				read(tuberia_re_em[0], mensaje, MAX);
			} while (strcmp(mensaje, "LISTO") != 0);

		} while (1);
		
		//Close Pipes
		close (tuberia_em_re[0]);
		close (tuberia_em_re[1]);
		close (tuberia_re_em[0]);
		close (tuberia_re_em[1]);
		
		//END
		return 0;
	}
}