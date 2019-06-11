#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_NAME		256
#define SERVER_IP 		"127.0.0.1"
#define SERVER_PORT		8888
#define MAX_CLIENTS		5
#define MAX_ITERATIONS	10 

#define READ	101	
#define WRITE 	102
#define SEND	201
#define OK		202

typedef unsigned char uchar;
typedef unsigned long ulong;

typedef struct part {
	ulong id;
	char name[MAX_NAME];
	ulong quantity;
} CarPart;

#endif
