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

	int sockfd,newsockfd,portno,n,clilen;
	char buffer[256];

	if(argc<2)
	{
		printf("Port number not provided\n");
		exit(1);
	}

	struct sockaddr_in serv_addr,cli_addr;
	//socklen_t clilen;

	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("Error Opening Socket");
		exit(1);
	}

	bzero((char *) &serv_addr,sizeof(serv_addr));
	//portno= 5001;
	portno=atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	

	//binding
	if( bind(sockfd, (struct sockaddr *) &serv_addr ,sizeof(serv_addr)) <0)
	{
		error("Binding Failed");
		exit(1);
	}
	//listen
	listen(sockfd,5);
	clilen= sizeof(cli_addr);

	newsockfd=accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0)
	{
		error("Error on accept");
		exit(1);
	}

	while(1)
	{
		bzero(buffer,256);
		n=read(newsockfd, buffer,255);
		if(n<0)
		{
			error("Error on Reading");
			exit(1);
		}
		//printf("Client : %s\n",buffer);
		int j=strcmp("Hello:B160769CS",buffer);
		int k=strcmp("Hello:b160769cs",buffer);
		if(j==0 || k==0)
		{
			strcpy(buffer,"This number belongs to NITC S6 student");
			n=write(newsockfd ,buffer,strlen(buffer));
			
		}
		else
		{
			strcpy(buffer,"Ok");
			n=write(newsockfd ,buffer,strlen(buffer));
		}
		if(n<0)
		{
			error("Error on Writing");
			exit(1);
		}
		
		if(j==0)
			break;
	}
	close(newsockfd);
	close(sockfd);

	return 0;

}