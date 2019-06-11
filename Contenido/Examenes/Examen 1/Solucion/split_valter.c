/*
    Valter Nunez
     A01206138
    Examen Final
*/


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

//Procedure listing
void list (char *base, char *lesser, char *greater, char* program, long *n, int recursive) {
    
    //Variable declaration
    DIR* dir;
    struct dirent* direntry;
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];
	long value;
    
    //Error Detection LESSER
    if ( (dir = opendir(lesser)) == NULL ) {
		perror(program);
		exit(-1);
	}
	
	//Error Detection GREATER
    if ( (dir = opendir(greater)) == NULL ) {
		perror(program);
		exit(-1);
	}
	
	//Error Detection BASE
    if ( (dir = opendir(base)) == NULL ) {
		perror(program);
		exit(-1);
	}	
	
	
	//Recursive
	rewinddir(dir);
	//Cycle to move
	while ((direntry = readdir(dir)) != NULL){
		if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0){
			sprintf(filename, "%s/%s", base, direntry->d_name);
			lstat(filename, &info);

			if (!S_ISDIR(info.st_mode)){
				value = info.st_size;

				if (value <= n){
					//Mover a Lesser
				}
				else {
				    //Mover a Greater
				}
			}
		}

	}	
	printf("\n");
}

int main(int argc, char* argv[]) {
        
    //Variable declaration
    char *base; 
    char *lesser;
    char *greater;
    long *n; 
    int recursive = 1;
    
    //Error detection
    if (argc != 5) {
        printf("usage: %s base lesser greater N\n", argv[0]);
        return -1;
    }
    
    //SCANS
	base = argv[1];
	lesser = argv[2];
	greater = argv[3];
	n = argv[4];
	
	list(base, lesser, greater, argv[0], n, recursive);
	
	return 0;
    
}
