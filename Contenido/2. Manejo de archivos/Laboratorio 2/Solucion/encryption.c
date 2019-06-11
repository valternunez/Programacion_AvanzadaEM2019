/*
        Manejo de Archivos
    Valter Núñez - A01206138
        Febrero 1 de 2019
*/


//IMPORTS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//MAIN
int main(int argc, char* argv[]) {
    
    //Declaración de variables
    int fd_in, fd_out;
    char *x;
    
  //MENSAJES DE ERROR
    //Usage number origin destination
    if (argc != 4){
        printf("Usage: number origin destination. \n");
        return -1;
    }
    
    //The first parameter must be a positive integer number.
    if(atoi(argv[1]) < 1 || atoi(argv[1]) > 128){
        printf("Encryption: the first parameter must be a positive integer number. \n");
        return -1;
    }
    
    //Otro errores
    if ( (fd_in = open(argv[2], O_RDONLY)) < 0){
        perror(argv[0]);
        return -1;
    }
    
    if ( (fd_out = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0){
        perror(argv[0]);
        return -1;
    }
    
    //Declaración Variables Encriptacion
    int i = 0;
    int j = 0;
    char buffer[128];
    char buff2 [128];
	long n = atoi(argv[1]);
    ssize_t b;
    
    //Proceso de encriptacion
    while ((b = read(fd_in, buffer, sizeof(buffer))) != 0) {
        if (b == 128){
            for (i = 0; (i+n) < 128; i++){
                buff2[i] = buffer[i+n];
            }
            
            for (j = 0; j<n; j++){
                buff2[i+j] = buffer[j];
            }
            
            write(fd_out,buff2,b);
        }
        else {
            write(fd_out,buffer,b);
        }//END ELSE
    }//END WHILE
    
    //Success
    
    //Cerrar DESCRIPTOR archivos
    close(fd_in);
    close(fd_out);
    
    printf("Done! \n");
    
    return 0;
    
}//END Main
