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

void get_filename(char filename[],char buffer[]){
int j=5;
int i =0;
for(i = 0 ; buffer[j]!='\n';i++,j++)
	filename[i] = buffer[j];
//filename[i]='1';
//i++;
filename[i]='\0';
}
int conn(int port){

	int sockid;
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
	serveraddr.sin_port=htons(port);



	//connection to the server
	if(connect(sockid,(struct sockaddr *)&serveraddr , sizeof(serveraddr))){
		printf("Connection to Server Failed!\n");
		exit(0);
	}
	printf("Connection to Server Successful...\n");
return sockid;
}
void comm_client_to_server(int sockid){
	char buffer[100]={'\0'};
	/*recv (sockid,buffer,sizeof(buffer),0);
	
	while(strcmp(buffer,"\0")!=0){
		if(strcmp(buffer,"conn 20023\n")==0)
			conn(PORT2);
		else if(strcmp(buffer,"conn 20020\n")==0)
			conn(PORT3);
		else
			printf("1");
	bzero(buffer,sizeof(buffer));
	//recv(sockid,buffer,sizeof(buffer),0);

	}
*/
	//bzero(buffer,sizeof(buffer));
	char *c= fgets(buffer,sizeof(buffer),stdin);
	int sock_port1 =-1;
	int sock_port2 = -1;
	while(strcmp(buffer,"goodbye server\n")!=0){
				
		printf("%s",buffer);
		/*char ret[6];
		for(int i = 0;i<5;i++){
			ret[i] = buffer[i];
		}
		ret[5]='\0';
		*/if(strncmp(buffer,"retr ",5)==0){
			//printf("1*");
			char filename[100];
			get_filename(filename,buffer);
			//printf("%s",filename);
			//FILE *fileptr = fopen(filename , "w");
		
			send(sockid,buffer,sizeof(buffer),0);
			//printf("%s",buffer);
			bzero(buffer,sizeof(buffer));

			recv(sockid,buffer,sizeof(buffer),0);
			
			//printf("++%s",buffer);
			if(strcmp(buffer,"conn 20020\n")==0){			FILE* fileptr = fopen(filename,"w");
				//printf("aaa");
				sock_port1 = conn(PORT3);
				bzero(buffer,sizeof(buffer));
				recv(sock_port1, buffer,sizeof(buffer),0);
			
				while(strcmp(buffer,"-1\n")!=0){
					fprintf(fileptr,"%s",buffer);
					bzero(buffer,sizeof(buffer));
					recv(sock_port1,buffer,sizeof(buffer),0);
					}
//ME
bzero(buffer,sizeof(buffer));
recv(sock_port1,buffer,sizeof(buffer),0);
printf("%s",buffer);
//me


				bzero(buffer,sizeof(buffer));
				close(sock_port1);
				fclose(fileptr);
				}

			else if(strcmp(buffer,"conn 20023\n")==0){
				printf("bbb");
				//sock_port2 = conn(PORT2);
//me
sock_port2 = conn(PORT3);
//
				//bzero(buffer,sizeof(buffer));
				recv(sock_port2,buffer,sizeof(buffer),0);
				printf("%s",buffer);
				bzero(buffer,sizeof(buffer));
				close(sock_port2);
				}
			bzero(buffer,sizeof(buffer));
			}
			
		else
		{
			send(sockid,buffer,sizeof(buffer),0);
			bzero(buffer,sizeof(buffer));
			recv(sockid,buffer,sizeof(buffer),0);
		}	
	fgets(buffer,sizeof(buffer),stdin);
	}
	send(sockid,buffer,sizeof(buffer),0);
	bzero(buffer,sizeof(buffer));
	recv(sockid,buffer,sizeof(buffer),0);
	printf("%s",buffer);
	if(sock_port1>0)	
		close(sock_port1);
	if(sock_port2>0)	
		close(sock_port2);
	
	
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

