/*
    Valter Núñez A01206138
    
    Manejo de Sockets
    
    Based on:
        Sockets by Manchas2k4 --> https://github.com/Manchas2k4/advanced_programming/tree/master/sockets
*/

//Includes
#include "header.h"

//Main function
int main(int argc, char* argv[]) {
    
    //Variable Declaration
    int sfd, number, answer, continuE;
	int port;
	struct sockaddr_in server_info;
	
	//Checks if the usage of the file is correct (string then ipport)
	if (argc != 3) {
	    printf("usage: %s ip port\n", argv[0]);
	    return -1;
	}
	
	//change from string to integer
	port = atoi(argv[2]);
	
	//No estoy seguro
	if (port < 5000) {
		printf("%s: The port must be greater than 5000.\n", argv[0]);
		return -1;
	}
	
	//Error detection when creating a socket
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	//Get information from the client now
	server_info.sin_family = AF_INET;    //Family
	server_info.sin_addr.s_addr = inet_addr(argv[1]);   //IP Address
	server_info.sin_port = htons(port);   //Port connecting
	
	//Error detection when connecting to the server
	if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	
	//VARIABLES NO DECLARADAS
	//text and continue
	char text[500];
	int cont = 0;
	char reply[500];

	 //Get info from the user by terminal
	 printf("Give me a text :\n");
	 fgets(text, sizeof(text), stdin);
	    
	 //Send and recieve from server
	 write(sfd, &text, sizeof(text));
	  
	 read(sfd, &reply, sizeof(reply));
	 
	 printf("The text you sent is: %s", reply);
	    
    scanf("%i", &cont);
	
	//Close connection
	close(sfd);
	
	//Return 0 
	return 0;
}