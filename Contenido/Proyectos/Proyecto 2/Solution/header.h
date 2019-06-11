#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/socket.h>


#define MUTEX	0

#define HELLO			1
#define SEND_FILE		101
#define SHOW_DIR		102
#define FIN		103
#define NO_PERM	201
#define FILE_NF	202
#define INTERNAL_ERROR		203
#define COMMAND_NF	204
#define IS_DIR		205
#define SENDING_F	301
#define DIR_NF	206
#define NOT_DIR		207
#define SENDING_DIR	302

void snd_msg(int sfd, int code, char* data) {
	long length = strlen(data);

	write(sfd, &code, sizeof(code));
	write(sfd, &length, sizeof(length));
	write(sfd, data, length * sizeof(char));
}
#endif
