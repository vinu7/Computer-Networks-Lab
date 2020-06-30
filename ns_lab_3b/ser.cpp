#include<cstdio>
#include<stdlib.h>
#include<cstring>
#include<unistd.h>
#include<time.h>
#include<signal.h>


#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define PORT1 20021
#define PORT2 20023
#define PORT3 20020

#define QUEUE_LIMIT 4

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


int file_transfer(char * filename, int sock_port1,FILE* fileptr)
{

	char buffer[100];
	bzero(buffer,sizeof(buffer));
	while(fgets(buffer,sizeof(buffer),fileptr)!=NULL)
	{
		send(sock_port1,buffer,sizeof(buffer),0);
		bzero(buffer,sizeof(buffer));		

	}	

	strcpy(buffer,"-1\n");
	send(sock_port1,buffer,sizeof(buffer),0);

	return 0;
}


void getFileCreationTime(char path[])
{
    struct stat attr;
    stat(path, &attr);
    printf("Last modified time: %s", ctime(&attr.st_mtime));
}

void comm_server_to_client(int socknew)
{
	
	char buffer[100];
	int i,j;
	
	char filename[100];
	char http_version[100];
	char relpath[100];
			
	
	int sock_port1=-1;//20020
	int sock_port2=-1;//20023
	while(1)//strcmp(buffer,"goodbye server\n")!=0)
	{
		//printf("%s",buffer);
		bzero(buffer,sizeof(buffer));
		recv(socknew,buffer,sizeof(buffer),0);
		printf("%s",buffer);
		get_filename(filename,relpath,http_version,buffer);
			
		if(strncmp(buffer,"GET ",4)==0)
		{
			//printf("%s",buffer);
			//strcpy(filename,"test.txt");
			//printf("-----");
			printf("%s",filename);
			if(strcmp(buffer,"GET / HTTP/1.1\n"))
			{
				//trsnfer index.html
				printf("transfer index.html\n");
				bzero(buffer,sizeof(buffer));
				FILE* fileptr = fopen("index.html","r");
				
				int fileval = file_transfer("index.html",socknew,fileptr);
				bzero(buffer,sizeof(buffer));
					//close(sock_port1);
				fclose(fileptr);			
				
			}
			else
			{
				printf("relpath is %s",relpath);

				FILE* fileptr = fopen(relpath,"r");
				if(fileptr != NULL)
				{
					printf("1");
					//int sock_port1 = conn(PORT3,socknew);
					bzero(buffer,sizeof(buffer));
					strcpy(buffer,"HTTP/1.1 200 ok\n");
					send(socknew,buffer,sizeof(buffer),0);
					bzero(buffer,sizeof(buffer));

					int fileval = file_transfer(filename,socknew,fileptr);
					bzero(buffer,sizeof(buffer));
					//close(sock_port1);
					fclose(fileptr);			
				}
				else
				{
					printf("FILe not found");

					//“HTTP/1.1 404” if the file is not found
					bzero(buffer,sizeof(buffer));
					strcpy(buffer,"HTTP/1.1 404\n");
					send(socknew,buffer,sizeof(buffer),0);

				}

				
			}
			
		}
		else if(strncmp(buffer,"HEAD ",5)==0)
		{
			printf("head action\n");
			getFileCreationTime(relpath);


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
		
		//recv(socknew,buffer,sizeof(buffer),0);
	}


	bzero(buffer,sizeof(buffer));
	strcpy(buffer,"Thank You , All connections successfully terminated!\n");
	send(socknew,buffer,sizeof(buffer),0);
	close(socknew);
}

int main(){

	int sockid,len,socknew;
	struct sockaddr_in serveraddr,clientaddr;



	//socket creation
	if(!(sockid = socket(AF_INET,SOCK_STREAM,0))){
		printf("Socket Creation Failed!\n");
		exit(0);		
	}
	printf("Socket Creation Successful...\n");
	bzero(&serveraddr,sizeof(serveraddr));
	
	int opt =1;
	// Forcefully attaching socket to the port 8080 
    	if (setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
   	 { 
        printf("setsockopt"); 
        exit(EXIT_FAILURE); 
   	 } 
	//setting the server address
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT1);

	

	//binding port to the socket
	if(bind(sockid,(struct sockaddr *) &serveraddr,sizeof(serveraddr))!=0){
		printf("Socket Binding Failed!\n");
		exit(0);
	}
	printf("Binded to Socket Successfully...\n");
	


	//port listening
	if(listen(sockid,QUEUE_LIMIT)){
		printf("Listening Failed!\n");
		exit(0);
	}
	len= sizeof(clientaddr);
	printf("Server Listening...\n");


	//port accept connection
for(;;){	
	if(!(socknew=accept(sockid,(struct sockaddr *) &clientaddr,(socklen_t*)&len))!=0){
		printf("Server Accepting Failed!\n");
		exit(0);
	}

	signal(SIGPIPE,SIG_IGN);
	printf("Server Accepted the client...\n");
	int c = fork();
	int childpid=-1;
	//printf("sksk");
	if((childpid =fork()) == 0)
	//communication between server and client communication
	{//closes the listening port on the child process
	close(sockid);	
	comm_server_to_client(socknew);	
	break;
	}
}
close(sockid);
return 0;
}