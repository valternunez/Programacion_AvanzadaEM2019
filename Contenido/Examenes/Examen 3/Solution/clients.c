#include "header.h"

void send_data(int sfd, uchar cmd, ulong id, ulong val) {
	write(sfd, &cmd, sizeof(uchar));
	write(sfd, &id, sizeof(ulong));
	write(sfd, &val, sizeof(ulong));
}

void receive_data(int sfd, uchar *cmd, ulong *val) {
	read(sfd, cmd, sizeof(uchar));
	read(sfd, val, sizeof(ulong));
}

// 101 (ask data), id, null (0) => 201 (send data), value
// 102 (write data), id, val => 202 (ok), new value)
void a_client() {
	int i, sfd;
	uchar cmd;
	ulong id, val;
	struct sockaddr_in server_info;
	
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket");
		exit(-1);
	}
	
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_info.sin_port = htons(SERVER_PORT);
	if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror("connect");
		exit(-1);
	}
	
	srand(getpid());
	for (i = 0; i < MAX_ITERATIONS; i++) {
		id = rand() % 5;
		
		if (rand() % 4 == 0) { // WRITE
			cmd = WRITE;	
			val = (rand() % 5) + 1;
		} else {
			cmd = READ;	
			val = 0;
		}
		
		printf("PID %i - cmd %u - id = %lu - val = %lu\n", getpid(), cmd, id, val);
		send_data(sfd, cmd, id, val);
		
		receive_data(sfd, &cmd, &val);
		printf("PID %i - cmd %u - val = %lu\n", getpid(), cmd, val);
	}
	
	close(sfd);
	
	exit(0);
}

int main(int argc, char* argv[]) {
	int i, pid, rid;
	
	for (i = 0; i < MAX_CLIENTS; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			a_client();
		} else {
			printf("PID = %i - I created the process %i\n", getpid(), pid);
		}
	}
	
	while (i > 0) {
		rid = wait(NULL);
		printf("The process %i has ended\n", rid);
		i--;
	} 
	return 0;
}
