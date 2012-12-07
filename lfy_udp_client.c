/************************************************************************
File name - lfy_udp_client.c
Purpose - An udp client, which accepts data from keyboard and sends the 
data to a server process.
To compile - gcc lfy_udp_client.c -o myclient
To run - ./myclient <server's IP address> <serever's port no>
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
#include	<fcntl.h>
#include	<signal.h>
#include	<unistd.h>
#define BUFSIZE 512
#define MAXSERVERNAME 100
int my_work(FILE *, int, struct sockaddr * ,socklen_t);

int main( int C, char *argv[] )
{
	int	sd, ret;
	struct	sockaddr_in serveraddress;
	if (NULL == argv[1])
	{
		printf("Please enter the IP Address of the server\n");
		exit(1);
	}
	if (NULL== argv[2])
	{
		printf("Please enter the Port Number of the server\n");
		exit(1);
	}
	sd = socket( AF_INET, SOCK_DGRAM, 0 );
	if(0 > sd ) 
	{
		perror( "socket" );
		exit (1);
	}
	
	memset( &serveraddress, 0, sizeof(serveraddress) );
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(atoi(argv[2]));//PORT NO
	serveraddress.sin_addr.s_addr = inet_addr(argv[1]);//ADDRESS
	
	printf("Client Starting service\n");

	ret = my_work(stdin,sd ,(struct sockaddr *)&serveraddress,
			sizeof(serveraddress));
	if (0 > ret)
	{
		printf("Client Exiting - Some error occured\n");
		close(sd);
		exit(1);
	}
	close(sd);
	exit(0);
}

/*********************************************************************
Function: - my_work (sends data to server process using udp socket)
Input - 
	a)input data source(here taken as stdin).
	b)socket descriptor.
	c)socket address structure, containing IP address and port number 
	of the server.
	d)length of the socket address structure.

Returns - 
	0 or -1 based on whether it exited successfully, when 
	there was end of file(means the user stopped sending data) or 
	if any error condition was encountered.
**********************************************************************/

int my_work(FILE *fp,		/*Here to be used as stdin as argument*/	 
		int sockfd ,
		struct sockaddr *to ,socklen_t length)	/*Connection Socket */
{

	char sendbuf[BUFSIZE], recvbuf[BUFSIZE],servername[MAXSERVERNAME];
	char *cptr;
	int ret, numbytes, slen;
	socklen_t structlen;
	struct sockaddr_in serveraddr;
	
	for(;;)
	{
		
		printf("Enter Data For the server or press CTRL-D to exit\n");
		/*Reading data from the keyboard*/
		cptr = fgets(sendbuf,BUFSIZE,fp);
		if (NULL == cptr)
		{
			printf("Possible error or end of file\n");
			return 0;
		}
		slen = strlen (sendbuf);
		/*Sending the read data over socket*/
		ret = sendto(sockfd,sendbuf,slen,0,to,length);
		if (0 > ret)
		{
			perror("Error in sending data:\n");
			return -1;
		}
		printf("Data Sent To Server\n");
		structlen = sizeof(serveraddr);
		numbytes = recvfrom(sockfd,recvbuf,BUFSIZE,0, (struct sockaddr*)&serveraddr,&structlen);
		if (0 > numbytes)
		{
			perror("Error in receiving data:\n");
			return -1;
		}
		printf("Data Received from server %s:\n", inet_ntop(AF_INET,&serveraddr.sin_addr,
				servername,sizeof(servername)));
		/*writing to stdout*/
		write(1,recvbuf,numbytes); 
	}

}

