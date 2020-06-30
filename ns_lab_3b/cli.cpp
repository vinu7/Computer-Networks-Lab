#include<cstdio>
#include<stdlib.h>
#include<unistd.h>
#include<cstring>

#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define PORT1 20021
#define PORT2 20023
#define PORT3 20020

void get_filename(char filename[],char relpath[],char http_version[], char buffer[])
{
	int j=4,k;
	int i =0;
	relpath[0]='.';
	relpath[1]='/';
	for(i = 0 ; (buffer[j]!=' ' && buffer[j]!='\n') ;i++,j++)
	{
		filename[i] = buffer[j];
		relpath[i+2]=buffer[j];
	}
	if(buffer[j]=' ')
	{
		for(k=0;buffer[j]!='\n';k++,j++)
		{
			http_version[k]=buffer[j];
		}
	}

	filename[i]='\0';
	relpath[i+2]='\0';
	http_version[k]='\0';
}



void comm_client_to_server(int sockid)
{
	char buffer[100]={'\0'};
	int sock_port1 =-1;
	int sock_port2 = -1;
	while(1)//strcmp(buffer,"goodbye server\n")!=0){
	{
		char *c= fgets(buffer,sizeof(buffer),stdin);
		
		printf("%s",buffer);
		send(sockid,buffer,sizeof(buffer),0);
			

		if(strncmp(buffer,"GET ",4)==0)
		{
			if(strcmp(buffer,"GET / HTTP/1.1\n")==0)
			{
				//default index.html
				bzero(buffer,sizeof(buffer));
				recv(sockid, buffer,sizeof(buffer),0);
				
				while(strcmp(buffer,"-1\n")!=0)
				{
					printf("%s",buffer);
					bzero(buffer,sizeof(buffer));
					recv(sockid,buffer,sizeof(buffer),0);
				}
				bzero(buffer,sizeof(buffer));
					
			}
			else
			{

		
				printf("1*");
				char filename[100];
				char http_version[100];
				char relpath[100];
				get_filename(filename,relpath,http_version,buffer);
				printf("%s\n",filename);
				printf("%s\n",http_version);

				printf("relpath is %s\n",relpath);

				recv(sockid,buffer,sizeof(buffer),0);

				if(strcmp(buffer,"HTTP/1.1 200 ok\n"))//strcmp(buffer,"conn 20020\n")==0)
				{

					//FILE* fileptr = fopen(filename,"w");
					printf("%s",buffer);
					bzero(buffer,sizeof(buffer));
					recv(sockid, buffer,sizeof(buffer),0);
				
					while(strcmp(buffer,"-1\n")!=0)
					{
						printf("%s",buffer);
						bzero(buffer,sizeof(buffer));
						recv(sockid,buffer,sizeof(buffer),0);
					}
					bzero(buffer,sizeof(buffer));
					//fclose(fileptr);
				}
				else if(strcmp(buffer,"HTTP/1.1 404\n"))//strcmp(buffer,"conn 20023\n")==0)
				{
					printf("bbb");
					//recv(sock_port2,buffer,sizeof(buffer),0);
					printf("%s",buffer);
					bzero(buffer,sizeof(buffer));
			
				}
				else
				{
					//
				}
			}
			
			
		}
		else if(strncmp(buffer,"HEAD ",5)==0)
		{
			printf("head action\n");


		}
		else if(strncmp(buffer,"PUT ",4)==0)
		{
			printf("PUT action\n");

		}
		else if(strncmp(buffer,"DELETE ",7)==0)
		{
			printf("DELETE action\n");

		} 
		else if(strncmp(buffer,"exit",4)==0)
		{
			printf("exit action\n");
			break;
		}

		else
		{
			/*bzero(buffer,sizeof(buffer));
			strcpy(buffer,"Please enter a valid command\n");
			send(socknew,buffer,sizeof(buffer),0);
			bzero(buffer,sizeof(buffer));
			*/
			printf("Enter valid input\n");
		}

	}
		
	
	
	return;
}

int main(){
	int sockid,socknew;
	struct sockaddr_in serveraddr,clientaddr;


	//socket creation
	if(!(sockid=socket(AF_INET,SOCK_STREAM,0))){
		printf("Socket Creation Failed!\n");
		exit(0);
	}
	printf("Socket Creation Successful...\n");
	bzero(&serveraddr,sizeof(serveraddr));



	//server address
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port=htons(PORT1);



	//connection to the server
	if(connect(sockid,(struct sockaddr *)&serveraddr , sizeof(serveraddr))){
		printf("Connection to Server Failed!\n");
		exit(0);
	}
	printf("Connection to Server Successful...\n");
	//communication from client to server
	comm_client_to_server(sockid);
	
	close(sockid);
return 0;
}

