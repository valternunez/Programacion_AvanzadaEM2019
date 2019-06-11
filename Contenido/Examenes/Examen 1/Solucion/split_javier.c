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
#include <ctype.h>
#define SIZE 128

void copy(char*basedir, char *destdir, char *program) {
    int src_fd, dst_fd, n, err;
    char buffer[SIZE];
	ssize_t nbytes;
	
	if ( (src_fd = open(basedir, O_RDONLY)) < 0 ) {
		perror(program);
		exit(-1);
	}

	if ( (dst_fd = open(destdir, O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(program);
		exit(-1);
	}

  	while ( (nbytes = read(src_fd, buffer, SIZE)) != 0 ) {
	    write(dst_fd, buffer, nbytes);
	}
	remove(basedir);
    close(src_fd);
    close(dst_fd);
}

void list(char *basedirectory, char *lessdirectory, char *greaterdirectory, int bytes, char* program) {
	DIR* bdir;
	DIR* ldir;
	DIR* gdir;
	struct dirent* direntry;
  	struct stat st;
	int size;
  	char filename[PATH_MAX + NAME_MAX + 1];
	char aux[PATH_MAX + NAME_MAX + 1];

  	if ((bdir = opendir(basedirectory)) == NULL ){
    	perror(program);
		exit(-1);
  	}
	if ((ldir = opendir(lessdirectory)) == NULL ){
		perror(program);
		exit(-1);
	}
	if ((gdir = opendir(greaterdirectory)) == NULL ){
		perror(program);
		exit(-1);
	}
	while ((direntry = readdir(bdir)) != NULL) {
		sprintf(filename, "%s/%s", basedirectory, direntry->d_name);
		lstat(filename, &st);
		size= (long long int)st.st_size;
		if (S_ISDIR(st.st_mode)){
            continue;
        }
		if (size>bytes){
			sprintf(aux, "%s/%s", greaterdirectory, direntry->d_name);
			copy(filename, aux, program);
		}
		else {
			sprintf(aux, "%s/%s", lessdirectory, direntry->d_name);
			copy(filename, aux, program);		
		}
  	}

	rewinddir(bdir);
  	while ((direntry = readdir(bdir)) != NULL ) {
    	if(strcmp(direntry->d_name,".")!=0&&strcmp(direntry->d_name,"..")!=0){
				sprintf(filename, "%s/%s", basedirectory, direntry->d_name);
				lstat(filename, &st);
				if (S_ISDIR(st.st_mode)) {
		    		list(filename, lessdirectory, greaterdirectory, bytes, program);
				} 	
	    }
	}
}

int main(int argc, char* argv[]) {
	char dir_name[PATH_MAX + 1];
	char *directory;
	int recursive=1, n;
	
	if (argc < 5) {
		printf("usage: %s base lesser greater N \n", argv[0]);
		return -1;
	}

	n=atoi(argv[4])*1000;
	if(n<1){
		printf("%s N must be a positive number greater than zero.\n", argv[0]);
		return -1;	
	}
	list(argv[1], argv[2], argv[3], n, argv[0]);
	
	return 0;
}
