/*
    Valter Núñez
    A01206138

    Proyecto Semestral 2

    ***
        Based on:
            Manchas2k4 Advanced Programming GitHub (several functions from his code) --> https://github.com/Manchas2k4/advanced_programming

*/

/*

  KNOWN PROBLEMS: The file and directory handling is incorrect. Gotta check.

*/

//Extra includes
#include "header.h"

//Declare Structure
typedef struct info_on_server {
    int nsfd, semid;
    char path[PATH_MAX + 1];
} strux;

//Check type of entry from client
int check_text(char* string){
    //If the string does not start with / (as part of Safety Procedures asked)
    if(string[0] != '/'){
        return 0;
    }

    //Check that the string sent by client does not have .. in it. (as part of the Safety Procedures asked)
    if (strstr(string, "..") != NULL){ //Validation taken from Peter Mortensen in Stack Overflow --> https://stackoverflow.com/questions/12784766/check-substring-exists-in-a-string-in-c
        return 0;
    }

    return 1;
}

//START --> SEMAPHORE HANDLING
int mutex_wait(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int mutex_signal(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int sem_wait(int semid, int sem_num, int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int sem_signal(int semid, int sem_num, int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}
//END --> SEMAPHORE HANDLING



//Create a Log of all the requests made by clients to server
void create_log(strux datax, int code, char* data, char ip[] ){
    //Wait
    mutex_wait(datax.semid, MUTEX);

    //Variable Declaration
    char filename[PATH_MAX + NAME_MAX + 1];
    time_t t;
    struct tm* tm;

    //Creation of the log.txt file
    sprintf(filename, "%s/%s", datax.path, "log.txt");

    //Assign values
    t = time(NULL);
    tm = localtime(&t); //Based on use of tm struct --> http://www.cplusplus.com/reference/ctime/tm/
    char date[100];
    strftime(date, 100, "%d / %m / %Y", tm);   //Based on the use of strftime --> https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm

    //Open File
    FILE* file = fopen(filename, "a");

    if(code == 0){
        fprintf(file, "\nFecha: %s ... IP: %s ... Data: %s \n", date, ip, data);
    } else {
        fprintf(file, "\nFecha: %s ... IP: %s ... Code: %i ... Data: %s \n", date, ip, code, data);
    }

    //Close file after finishing using it
    fclose(file);

    //Signal
    mutex_signal(datax.semid, MUTEX);
}

//Get information from client
int read_from_client(int sfd, char* data) {
  //Variable declaration
	int code;
	long length;
  long read_info;

  //Read data from the client
	read(sfd, &code, sizeof(code));
	read(sfd, &length, sizeof(length));

  //Read data into a variable.
	read_info = read(sfd, data, length * sizeof(char));
  data[length] = '\0';
  //Return code.
  if (read_info != length) {
		return -1;
	} else {
		return code;
	}
}


//Where the magic happens
void* serves_client(strux datax, char ip[] ){
    int code, nsfd, semid;
    char data [PATH_MAX + NAME_MAX + 1];
    char filename [PATH_MAX + NAME_MAX + 1];

    code = 0;
    //Create a stat variable to get the status of the files that the client wants to see.
    struct stat info;

    //Give to local function values, the values of the data structue sent from server function
    nsfd = datax.nsfd;
    semid = datax.semid;

    //Use the snd msg function declared in header.h
    snd_msg(nsfd, HELLO, "Hola");
    create_log(datax, HELLO, "Hola", ip);

    //While the client does't stop the connection
    do{

        //Recieve message
        code = read_from_client(nsfd, data);

        create_log(datax, code, data, ip);
        printf("CODE: %i DATA: %s\n", code, data);

        //If the code is invalid, throw error
        if(code == -1){
            snd_msg(nsfd, INTERNAL_ERROR, "Error Interno");
            create_log(datax, INTERNAL_ERROR, "Error Interno", ip);
            break;
        }

        //If the code is valid, start responding to requests
        switch (code){
            //Case if working with files
            case SEND_FILE:
                sprintf(filename, "%s%s", datax.path, data);
                lstat(filename, &info);  // -->https://linux.die.net/man/2/lstat

                //If the path sent by client has '/' or '..', get Internal Error
                if (check_text(data) == 0){
                    snd_msg(nsfd, INTERNAL_ERROR, "Error Interno");
                    create_log(datax, INTERNAL_ERROR, "Error Interno", ip);
                }
                printf("Filename: %s\n", filename);
                int aux;

                aux = 100;
                if (check_text(data) == 1){
                    printf("Check_text\n");
                    if (access(filename, F_OK) == -1){
                        printf("Archivo no encontrado\n");
                        snd_msg(nsfd,FILE_NF, "Archivo no Encontrado");
                        create_log(datax, FILE_NF, "Archivo no Encontrado", ip);
                    } else if (S_ISDIR(info.st_mode)){              // --> https://linux.die.net/man/2/stat
                        printf("Es directorio\n");
                        snd_msg(nsfd, 205, "Ruta es directorio");
                        create_log(datax, 205, "Ruta es directorio", ip);
                    } else if ((aux = access(filename, R_OK)) != 0){
                        printf("No tiene permiso, aux = %i\n", aux);
                        snd_msg(nsfd,NO_PERM, "Permiso Denegado");
                        create_log(datax, NO_PERM, "Permiso Denegado", ip);
                    } else {
                        printf("Archivo correcto, aux = %i\n", aux);
                        int fd;
                        fd = open(filename, O_RDONLY);
                        long lf = lseek(fd, 0, SEEK_END); // --> http://man7.org/linux/man-pages/man2/lseek.2.html
                        close (fd);

                        //Buffer declaration and assign
                        char* buffer;
                        buffer = (char*) malloc(lf * sizeof(char));

                        fd = open(filename, O_RDONLY);
                        long nb = read(fd, buffer, lf);

                        if (nb == lf){
                            //SENDFILE
                            int code = SENDING_F;
                            write(nsfd, &code, sizeof(code));
                            write(nsfd, &lf, sizeof(lf));
                            write(nsfd, &buffer, lf*sizeof(char));

                            create_log(datax, SENDING_F, "Enviando archivo", ip);
                        } else {
                            snd_msg(nsfd, INTERNAL_ERROR, "Error Interno");
                            create_log(datax, INTERNAL_ERROR, "Error Interno", ip);
                        }

                        free(buffer);
                        close(fd);
                    }
                }
                break;
            //Case working with directories
            case SHOW_DIR:
                sprintf(filename, "%s%s", datax.path, data);
                lstat(filename, &info);  // -->https://linux.die.net/man/2/lstat

                //If the path sent by client has '/' or '..', get Internal Error
                if (check_text(data) == 0){
                    snd_msg(nsfd,INTERNAL_ERROR, "Error Interno");
                    create_log(datax, INTERNAL_ERROR, "Error Interno", ip);
                }

                if (check_text(data) == 1){

                    if ( access (filename, F_OK) == -1){
                        snd_msg(nsfd, DIR_NF, "Directorio no encontrado");
                        create_log(datax, DIR_NF, "Directorio no encontrado", ip);
                    } else if (!S_ISDIR(info.st_mode)){         //-->https://linux.die.net/man/2/stat
                        snd_msg(nsfd, NOT_DIR, "Ruta no es directorio");
                        create_log(datax, NOT_DIR, "Ruta no es directorio", ip);
                    } else if (access(filename, R_OK|W_OK) == -1) {
                        snd_msg(nsfd, NO_PERM, "Permiso denegado");
                        create_log(datax, NO_PERM, "Permiso denegado", ip);
                    } else {
                        DIR* dir;
                        dir = opendir(filename);
                        struct dirent* direntry;
                        struct stat temps;
                        char tempf[NAME_MAX + 1];

                        sprintf(tempf, "%dtemp.txt", getpid());

                        FILE* arch = fopen(tempf, "a");

                        //Taken from manchas 2k4 direcory code --> https://github.com/Manchas2k4/advanced_programming/blob/master/directories/dir.c
                        while ((direntry = readdir(dir)) != NULL) {
                            if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0 && strstr(direntry->d_name, "temp.txt") == NULL){

                                fprintf(arch, "%s \n", direntry->d_name);
                            }
                        }

                        fclose(arch);
                        closedir(dir);

                        int fd;

                        fd = open(tempf, O_RDONLY);
                        long lf = lseek(fd, 0, SEEK_END);
                        close (fd);

                        char* buffer;
                        buffer = (char*) malloc(lf * sizeof(char));

                        fd = open(tempf, O_RDONLY);
                        long nb = read(fd, buffer, lf);

                        if (nb == lf){
                            //SEND FILE
                            int code = SENDING_DIR;
                            write(nsfd, &code, sizeof(code));
                            write(nsfd, &lf, sizeof(lf));
                            write(nsfd, &buffer, lf*sizeof(char));

                            create_log(datax, SENDING_DIR, "Enviando contenido en directorio", ip);
                        } else {
                            snd_msg(nsfd, INTERNAL_ERROR, "Error Interno");
                            create_log(datax, INTERNAL_ERROR, "Error Interno", ip);
                        }
                        free(buffer);
                        close(fd);
                        remove(tempf);
                    }
                }

                break;

            //End connection
            case FIN:
                create_log(datax, FIN, "Fin de Conexion", ip);
                break;
            //None of the above
            default:
                snd_msg(nsfd, COMMAND_NF, "Comando no conocido");
                create_log(datax, COMMAND_NF, "Comando no conocido", ip);
                break;
        }
    } while (code != FIN);

    close(nsfd);
}

void server (char* ip, int port, int semid, char* path, char* program){
    //Variable declaration
    int pid, sfd, nsfd;
    strux datax; //Use global struct
    struct sockaddr_in server_info, client_info;

    strcpy(datax.path, path);
    datax.semid = semid;

    //Create socket
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(program);
		exit(-1);
	}

    //Get information from server
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);

	//Bind the server info
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(program);
		exit(-1);
	}

    //Start waiting for a client to connect
    listen(sfd, 1);

    while(1){
        //Accept the connection of the client
        int len = sizeof(client_info);
		    if ( (datax.nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
             perror(program);
			       exit(-1);
		    }

    		//Concurrent connection
    		if ( (pid = fork()) < 0 ) {
    			perror(program);
    		} else if (pid == 0) {
    			close(sfd);
          create_log(datax, 0, "Conexion Exitosa", ip);
    			serves_client(datax, ip);
    			exit(0);
    		} else {
    			close(datax.nsfd);
    		}

    }

}

//Main declared correct...
int main(int argc, char* argv[]) {
    //Variable Declaration
    char ip[15];
    int port, semid;
    key_t erebor; //Key for semaphoresS


    if (argc == 6) {
  		if (strcmp(argv[1], "-d") == 0) {
  			strcpy(ip, argv[2]);
  			if (strcmp(argv[3], "-p") == 0) {
  				port = atoi(argv[4]);
  				if (port < 5000) {
  			    		printf("%s: The port must be greater than 5000.\n", argv[0]);
  			    		return -1;
  			    	}
  			} else {
  				printf("usage: %s -d ip -p port path\n", argv[0]);
  				return -1;
  			}
  		} else if (strcmp(argv[1], "-p") == 0) {
  			port = atoi(argv[2]);
  			if (port < 5000) {
  				printf("%s: The port must be greater than 5000.\n", argv[0]);
  				return -1;
  			}
  			if (strcmp(argv[3], "-d") == 0) {
  				strcpy(ip, argv[4]);
  			} else {
  				printf("usage: %s -d ip -p port path\n", argv[0]);
  				return -1;
  			}
  		} else {
  			printf("usage: %s -d ip -p port path\n", argv[0]);
  			return -1;
  		}
  	} else {
  		printf("usage: %s -d ip -p port path\n", argv[0]);
  		return -1;
  	}


	if ( (erebor = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}

	//SemaphoreID Get
	if ( (semid = semget(erebor, 1, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}

	//SetValue for MUTEX Semaphore
	semctl(semid, MUTEX, SETVAL, 1);

	//Send to server function
	server(ip, port, semid, argv[5], argv[0]);

	//Return 0
	return 0;

}
