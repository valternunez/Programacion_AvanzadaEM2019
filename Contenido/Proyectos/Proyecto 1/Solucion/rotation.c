//Proyecto Parcial

//Valter Nunez A01206138


//Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

//Rotation procedure
void rotate(char* program, char *path, char *filename, int number_of_files, int time){
  //Variable declaration
	char oldFile[PATH_MAX + NAME_MAX + 1], newFile[PATH_MAX + NAME_MAX + 1];
	int fd, i;

	while (1){
		sleep(time);

		for (i = number_of_files; i > 0; i--){
			sprintf(newFile, "%s/%s.%i", path, filename, i);

			if (i == 1){
				sprintf(oldFile, "%s/%s.log", path, filename);
			}
			else{
				sprintf(oldFile, "%s/%s.%i", path, filename, i - 1);
			}
      //ERROR Detection
			if ( (fd = open(oldFile, O_RDONLY)) >= 0 ){
				if ( (rename(oldFile, newFile)) < 0 ){
					perror(program);
					exit(-1);
				}
			}

		}

    //File management
		fd = open(oldFile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		close(fd);

		sprintf(oldFile, "%s/%s.%i", path, filename, number_of_files + 1);

		if ( (fd = open(oldFile, O_RDONLY)) >= 0 ){
			remove(oldFile);
		}
	}
}

//Main function
int main(int argc, char* argv[]){
  //Usage error
	if (argc != 5){
		printf("usage: %s working_directory log_filename number_of_files timeout \n", argv[0]);
		return -1;
	}

  //Variable declaration
	char *path;
	path = argv[1];
	DIR* dir;

  //Search for directory error
	if ( (dir = opendir(path)) == NULL ){
		printf("%s: Couldn't find directory: %s \n", argv[0], path);
		return -1;
	}

  //Permission error
	if (access(path,W_OK) || access(path,R_OK)){
		printf("%s: You don't have the required permissions in the directory: %s \n", argv[0], path);
		return -1;
	}

	char *x;
	int number_of_files;
	number_of_files = (int)strtol(argv[3], &x, 10);

  //# of Files validation error
	if (*x != '\0' || number_of_files < 1){
		printf("%s: the number_of_files parameter must be a positive integer number greater than zero.\n", argv[0]);
		return -1;
	}

	int time;
	time = (int)strtol(argv[4], &x, 10);

  //Time validation error
	if (*x != '\0' || time < 1){
		printf("%s: the time parameter must be a positive integer number greater than zero.\n", argv[0]);
		return -1;
	}

	char *filename;
	filename = argv[2];

  //Send to rotation procedure
	rotate(argv[0], path, filename, number_of_files, time);
	return 0;
}
