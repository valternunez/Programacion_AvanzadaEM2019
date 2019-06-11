/*
    Valter Núñez A01206138
    
    Manejo de Sockets
    
    Based on:
        Sockets by Manchas2k4 --> https://github.com/Manchas2k4/advanced_programming/tree/master/sockets
        

*/

//Includes
#include "header.h"
#include <string.h>


void serves_client(int nsfd, char *string) {
	
	char text_recieved[500];
	char reply[500];
	
	//Gets the information from the client
	read(nsfd, &text_recieved, sizeof(text_recieved));
	
	printf("%s: The text given is %s", string, text_recieved);

	//Send to the client the information 'proccessed' by the server
	write(nsfd, &text_recieved, sizeof(text_recieved));
	
	//Close connection to the client
	close(nsfd);

}


//Server function
void server(char* ip, int port, char* program) {
    //Variable Declaration
	int sfd, nsfd, pid;
	struct sockaddr_in server_info, client_info;
	struct in_addr client_ip;
	char string[15];

    //Error creating sockets. 
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	//Obtaining info of the server
	server_info.sin_family = AF_INET;   //Server family connection
	server_info.sin_addr.s_addr = inet_addr(ip);   //IP address from the server
	server_info.sin_port = htons(port);    //port connected of the server
	
	//Bind the information by cast 
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	//Listen to get the 'request' from the client
	listen(sfd, 1);
	
	
	while (1) {
	    
		int len = sizeof(client_info);
		//Accepts the request gotten from the client. 
		if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			perror(program);
			exit(-1);
		}
		
		client_ip = client_info.sin_addr;
		strcpy(string, inet_ntoa(client_ip));
		
		//Sends request to function that handles the 'brain' of the server
		serves_client(nsfd, string);  //ITERATIVO
	}
}


//Main function
int main(int argc, char* argv[]) {
    
    //Variable declaration
	char ip[15];	
	int port;
	
	//Get the info from header.h to this specific file.
	strcpy(ip, DEFAULT_IP);
	port = DEFAULT_PORT;
	
	//Go to 'run server' function
	server(ip, port, argv[0]);
	
	return 0;
}	