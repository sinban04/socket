#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/socket.h> 
#include <sys/un.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#define MAXLINE 1024
#define SOCKET_ADDRESS "/home/pi/socket.dat"

int main(int argc, char **argv)
{
	int server_sockfd, client_sockfd;
	int state, client_len;
	pid_t pid;

	FILE *fp;
	struct sockaddr_un clientaddr, serveraddr;

	char buf[MAXLINE];

	/*
	if (argc != 2)
	{
		printf("Usage : %s [socket file name]\n", argv[0]);
		printf("예    : %s /tmp/mysocket\n", argv[0]); 
		exit(0);
	}
	*/

	if (access(SOCKET_ADDRESS, F_OK) == 0)
	{
		unlink(SOCKET_ADDRESS);
	}
	// make internet-based stream socket
	
	client_len = sizeof(clientaddr);
	if ((server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		perror("socket error : ");
		exit(0);
	}
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, SOCKET_ADDRESS);

	state = bind(server_sockfd , (struct sockaddr *)&serveraddr, 
			sizeof(serveraddr));
	if (state == -1)
	{
		perror("bind error : ");
		exit(0);
	}

	state = listen(server_sockfd, 5);
	if (state == -1)
	{
		perror("listen error : ");
		exit(0);
	}

	while(1)
	{
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, 
				&client_len);
		pid = fork();
		if (pid == 0)
		{
			if (client_sockfd == -1)
			{
				perror("Accept error : ");
				exit(0);
			}
			while(1)
			{
				memset(buf, 0x00, MAXLINE);
				if (read(client_sockfd, buf, MAXLINE) <= 0)
				{
					close(client_sockfd);
					exit(0);
				}
				write(client_sockfd, buf, strlen(buf));
			}
		}
	}
	close(client_sockfd);

}
