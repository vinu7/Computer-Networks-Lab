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

	
	FILE *fp;

	int words,cnt=0;
	fp=fopen("RollS.txt","w");

	read(newsockfd ,&words,sizeof(int));

	while(cnt!=words)
	{
		read(newsockfd, buffer ,256);
		fprintf(fp, "%s" ,buffer);
		cnt++;
	}

	

	printf("server done\n");

	fclose(fp);
	fp=fopen("RollS.txt","r");
	
	fgets(buffer, sizeof(buffer), fp);
	
	printf("%s",buffer);

	fclose(fp);
	
	
	write(newsockfd,buffer,strlen(buffer));
	



	close(newsockfd);
	close(sockfd);

	return 0;

}
