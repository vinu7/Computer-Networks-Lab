#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char *str)
{
	perror(str);
	exit(1);
}

int main(int argc,char *argv[])
{

	int sockfd,portno,n;
	char buffer[256];
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if(argc<3)
	{
		printf("We must need three arguments\n");
		exit(1);
	}

	portno=atoi(argv[2]);
	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("erroe opening socket");
		exit(1);
	}

	server = gethostbyname(argv[1]);
	if(server==NULL)
	{
		printf("Error,no such hhost");
		exit(1);
	}

	bzero((char *) &serv_addr ,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char *) server->h_addr , (char *) &serv_addr.sin_addr.s_addr ,server->h_length);
	serv_addr.sin_port =htons(portno);

	if(connect(sockfd, (struct sockaddr *) &serv_addr ,sizeof(serv_addr)) <0)
		error("Connection failed");
	while(1)
	{
		bzero(buffer,256);
		//fgets(buffer,255,stdin);
		printf("Enter Message: ");
		scanf("%s",buffer);
		n=write(sockfd,buffer,strlen(buffer));

		if(n<0)
			error("Error on writing");
		bzero(buffer,256);
		n=read(sockfd,buffer,255);
		if(n<0)
			error("Error on reading");
		//printf("Server: %s\n",buffer);
		int j=strcmp("This number belongs to NITC S6 student",buffer);
		if(j==0)
			break;
	}

	close(sockfd);
	return 0;
}

