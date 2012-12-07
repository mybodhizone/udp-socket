/************************************************************************
File name - lfy_udp_server.c
Purpose - An udp server, which accepts data sent by a client,
and echoes back the same data to the client.
To compile - gcc lfy_udp_server.c -o myserver
To run - ./server 
Port Number of  server hardcoded as 11710 (can be changed)
************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<sys/wait.h>
#include	<fcntl.h>
#include	<signal.h>

#define BUFSIZE 512
#define MYPORT 11710
#define MAXNAME 100 
int main(int argc,  char *argv[] )

{
	int	sd, numbytes,bytessent, ret;
	struct	sockaddr_in
		serveraddress,cliaddr;
	socklen_t length;
	char clientname[MAXNAME],datareceived[BUFSIZE];

	sd = socket( AF_INET, SOCK_DGRAM, 0 );
	if(0 > sd ) 
	{
		perror( "socket" );
		exit( 1 );
	}
	

	memset( &serveraddress, 0, sizeof(serveraddress) );
	memset( &cliaddr, 0, sizeof(cliaddr) );
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(MYPORT);//PORT NO
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);//IP ADDRESS
	ret=bind(sd,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
	if(0 > ret)
	{
		perror("Bind Fails:");
		exit(1);
	}
	for(;;)
	{
		printf("UDP Server:  Waiting for client data\n");
		length=sizeof(cliaddr);
		/*Received a datagram*/
		numbytes = recvfrom(sd,datareceived,BUFSIZE,0,
				(struct sockaddr*)&cliaddr, &length);
		if (0 > numbytes)
		{
			perror("Error while receiving:");
			exit(1);
		}
		/*Printing client's address*/
		printf("Data Received from %s\n",
				inet_ntop(AF_INET,&cliaddr.sin_addr,
					clientname,sizeof(clientname)));
		
		/*Sending the Received datagram back*/
		datareceived[numbytes]='\0';
		printf("Server Receives: %s\n",datareceived);
		bytessent = sendto(sd,datareceived,numbytes,0,(struct sockaddr *)&cliaddr,length);
		if (0 > bytessent)
		{
			perror("Error while sending:");
			exit(1);
		}
	}
}

