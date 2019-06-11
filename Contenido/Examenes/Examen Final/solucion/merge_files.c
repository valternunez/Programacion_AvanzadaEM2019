/*
    EXAMEN FINAL -- PROGRAMACIÓN AVANZADA
    Valter Núñez - A01206138
    
    Based on: 
        My own code on Github --> https://github.com/valternunez/Programacion_AvanzadaEM2019
    

*/

//Includes
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
#include <fcntl.h>
 

void get_info(char* name, char* directory, int mergef){
    
    //Variable declaration
    int i; 
    struct stat info;
    char filename[PATH_MAX + NAME_MAX + 1];
    int file;
    sprintf(filename, "%s/%s", directory, name);
    int x;
    
    //Error detection
    if (lstat (filename, &info) < 0) {
		perror("lstat");
		exit(-1);
	}
	
	//Check if the thing we are looking at is or not a directory
	if (S_ISDIR(info.st_mode)){
	    return;
	}
	
	//If it is a file, but you don't have permissions, RIP
	if ( (file = open(filename, O_RDWR)) < 0){
        printf("%s cannot be read ...\n", filename);
        return;
	}
	
	//If everything is peachy, start merging
	printf("Merging %s\n", filename);
	
	char* buffer = (char*) malloc(128); 
	
	while ( (x = read(file, buffer, 128)) != 0){  //Based on the documantation of read --> http://codewiki.wikidot.com/c:system-calls:read
	    write(mergef, buffer, x);  // --> http://codewiki.wikidot.com/c:system-calls:write
	}
	
}

//Merge procedure
void merge(char* directory, int recursive, int file, int max_level, int aux, char* program){
    //Variable declaration
    DIR* dir;
    struct dirent* direntry;
    struct stat info;
    char filename[PATH_MAX + NAME_MAX +1];
    
  //VALIDATIONS 
    //Check directory exists
    if ( (dir = opendir(directory)) == NULL){
        perror("error");
        exit(-1);
    }
    
    //Send to 'file validation' of sorts
    while ( ( direntry = readdir(dir)) != NULL){
        get_info(direntry->d_name, directory, file);
    }
    
    //Check that it can be recursive and if the current level is valid or we are finished
	if (recursive && aux < max_level) {
		rewinddir(dir);
		while ( (direntry = readdir(dir)) != NULL ) {
		    
			if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) {
			    
				sprintf(filename, "%s/%s", directory, direntry->d_name);
				lstat(filename, &info);
				
				if (S_ISDIR(info.st_mode)) {
				    aux = aux + 1;
					merge(filename, recursive, file, max_level, aux, program);
				}
			}
		}
	}
    
}

//Main
int main (int argc, char* argv [] ){
    
    //Variable declaration
    int max_level;
    char* directory;
    int file, aux;
    
  //VALIDATIONS
    //Amount of inputs
    if (argc != 4){
        printf("usage: %s directory merge_file max_level\n", argv[0]);
        return -1;
    }
    
    
    max_level = atoi(argv[3]);
    //Max level greater than 0
    if (max_level < 1){
        printf("%s: The max_level must be a positive integer number greater than or equal to 1\n", argv[0]);
        return -1;
    }
    
    //Check you can create the file
    if ( (file = open(argv[2], O_RDWR)) < 0){
        printf("Can't find file");
        return -1;
    }
    
    directory = argv[1];
    
    aux = 1;
    merge(directory, 1, file, max_level, aux, argv[0] );
    close(file);
    
}