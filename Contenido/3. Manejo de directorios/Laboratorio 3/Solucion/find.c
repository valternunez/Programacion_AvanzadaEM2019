/*----------------------------------------------------------------

*

* Programación avanzada: Archivos y directorios

* Fecha: 12 de febrero de 2019

* Autor: A01206138 Valter Nunez

*

*--------------------------------------------------------------*/

//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

//Procedure obtain information -- DONE
void get_info (char *name, char *directory, char* program) {
    
    //Variable declaration
    int i; 
    struct stat info;
    time_t rawtime;
    struct tm* timeinfo;
    char filename[PATH_MAX + NAME_MAX + 1];
    sprintf(filename, "%s/%s", directory, name);
    
    //Error detection
    if (lstat (filename, &info) < 0) {
		perror(program);
		exit(-1);
	}

    //Show information stored
    printf("Nombre: %s \n", name);
	printf("PATH: %s \n\n", directory);
    
    //Last access
    rawtime = info.st_atime;
	timeinfo = localtime(&rawtime);
	printf("Ultima apertura: %s \n",asctime(timeinfo));
	
	
	//Last modification
	rawtime = info.st_mtime;
	timeinfo = localtime(&rawtime);
	printf("Ultima modificacion: %s \n\n", asctime(timeinfo));
}



//Procedure listing -- DONE
void list (char *directory, int recursive, char* program, char* str) {
    
    //Variable declaration
    DIR* dir;
    struct dirent* direntry;
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];
    
    //Error Detection
    if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}
	
	//Directory information
	while ( (direntry = readdir(dir)) != NULL ) {
		get_info(direntry->d_name, directory, program);
	}
	
	//Recursion instructions
	if (recursive) {
		rewinddir(dir);
		while ( (direntry = readdir(dir)) != NULL ) {
			if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) {
				sprintf(filename, "%s/%s", directory, direntry->d_name);
				lstat(filename, &info);
				if (S_ISDIR(info.st_mode)) {
					list(filename, recursive, program, str);
				}
			}
		}
	}
	printf("\n");
    
}

//Main function -- DONE
int main(int argc, char* argv[]) {
    
    //Variable declaration
    char *directory; 
    char *str; 
    int recursive = 0;
    
    //Error detection
    if (argc != 3 && argc != 4) {
        printf("usage: %s str [directory] [-r]\n", argv[0]);
        return -1;
    }
    
    //Scans
    str = argv[1];
	directory = argv[2];
    
    
    if (argc == 4)
	{
		if (strcmp(argv[3], "-r") == 0) 
		{
			recursive = 1;
		}
		else
		{
			printf("usage: %s str directory [-r]\n", argv[0]);
		}
	}
	
	list(directory, recursive, argv[0], str);
	
	
	return 0;
}
