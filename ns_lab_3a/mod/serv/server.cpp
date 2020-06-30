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

void get_filename(char filename[],char buffer[]){
int j=5;
int i =0;
for(i = 0 ; buffer[j]!='\n';i++,j++)
	filename[i] = buffer[j];
filename[i]='\0';
}

int file_transfer(char * filename, int sock_port1,FILE* fileptr){

	char buffer[100];
	bzero(buffer,sizeof(buffer));
	while(fgets(buffer,sizeof(buffer),fileptr)!=NULL){
	
	send(sock_port1,buffer,sizeof(buffer),0);
	bzero(buffer,sizeof(buffer));		

	}	

	strcpy(buffer,"-1\n");
	send(sock_port1,buffer,sizeof(buffer),0);

//me
strcpy(buffer,"FILE TRANSFER COMPLETED :CLOSING 2020\n");
send(sock_port1,buffer,sizeof(buffer),0);
//me
	return 0;
}

int conne(int port,int socknew_est){
	int sockid,len,sockid_ext;	
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
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    	} 
	

	//setting the server address
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port);

	

	//binding port to the socket
	if(bind(sockid,(struct sockaddr *) &serveraddr,sizeof(serveraddr))!=0){
		printf("Socket Binding Failed!\n");
		exit(0);
	}
	printf("Binded to Socket Successfully...\n");
	


		

	char buffer[100];
	//if(port==20023)
		strcpy(buffer,"conn 20023\n");
	//else 
	//	strcpy(buffer,"conn 20020\n");
	send(socknew_est,buffer,sizeof(buffer),0);
	
	bzero(buffer,sizeof(buffer));

	
	//port listening
	if(listen(sockid,QUEUE_LIMIT)){
		printf("Listening Failed!\n");
		exit(0);
	}
	len= sizeof(clientaddr);
	printf("Server Listening...\n");
	//port accept connection
	if(!(sockid_ext=accept(sockid,(struct sockaddr *) &clientaddr,(socklen_t*)&len))!=0){
		printf("Sever Accepting Failed!\n");
		exit(0);
	}
	printf("Server Accepted the client...\n");	
	close(sockid);
return sockid_ext;
}




int conn(int port,int socknew_est){
	int sockid,len,sockid_ext;	
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
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    	} 
	

	//setting the server address
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port);

	

	//binding port to the socket
	if(bind(sockid,(struct sockaddr *) &serveraddr,sizeof(serveraddr))!=0){
		printf("Socket Binding Failed!\n");
		exit(0);
	}
	printf("Binded to Socket Successfully...\n");
	


		

	char buffer[100];
	if(port==20023)
		strcpy(buffer,"conn 20023\n");
	else 
		strcpy(buffer,"conn 20020\n");
	send(socknew_est,buffer,sizeof(buffer),0);
	
	bzero(buffer,sizeof(buffer));

	
	//port listening
	if(listen(sockid,QUEUE_LIMIT)){
		printf("Listening Failed!\n");
		exit(0);
	}
	len= sizeof(clientaddr);
	printf("Server Listening...\n");
	//port accept connection
	if(!(sockid_ext=accept(sockid,(struct sockaddr *) &clientaddr,(socklen_t*)&len))!=0){
		printf("Sever Accepting Failed!\n");
		exit(0);
	}
	printf("Server Accepted the client...\n");	
	close(sockid);
return sockid_ext;
}

void comm_server_to_client(int socknew){
	char buffer[100];
	//printf("Enter the action:");
	//while(fgets(buffer,sizeof(buffer),stdin)!=NULL){
		
	//	if (strcmp(buffer,"conn 20020\n")==0){
	//		conn (PORT2,socknew);
	//		send(socknew,buffer,sizeof(buffer),0);}
	//	else if(strcmp(buffer,"conn 20023\n")==0){
	//		conn (PORT3,socknew);
	//		send(socknew,buffer,sizeof(buffer),0);}
int sock_port1=-1;//20020
int sock_port2=-1;//20023
	recv(socknew,buffer,sizeof(buffer),0);
	printf("%s",buffer);
	while(strcmp(buffer,"goodbye server\n")!=0){
		//printf("%s",buffer);
		
		if(strncmp(buffer,"retr ",5)==0){
			char filename[100];

			//printf("%s",buffer);
			get_filename(filename,buffer);
			//strcpy(filename,"test.txt");
			//printf("-----");
			printf("%s",filename);
			FILE* fileptr = fopen(filename,"r");
			if(fileptr != NULL){
			printf("1");
			int sock_port1 = conn(PORT3,socknew);
			
			bzero(buffer,sizeof(buffer));
	//		strcpy(buffer,"conn 20020");
	//		send(socknew,buffer,sizeof(buffer),0);
			bzero(buffer,sizeof(buffer));

			//returns -1 if no file found else return 0
			int fileval = file_transfer(filename,sock_port1,fileptr);
			bzero(buffer,sizeof(buffer));
			close(sock_port1);
			fclose(fileptr);			
			}
			else{
			printf("***");

			//int sock_port2 = conn(PORT2,socknew);
//me
int sock_port2 = conne(PORT3,socknew);
			bzero(buffer,sizeof(buffer));
	//		strcpy(buffer,"conn 20023");
	//		send(socknew,buffer,sizeof(buffer),0);
			bzero(buffer,sizeof(buffer));
			time_t rawtime;
  			struct tm * timeinfo;

  			time ( &rawtime );
  			timeinfo = localtime ( &rawtime );
			bzero(buffer,sizeof(buffer));


			strcpy(buffer,strcat(asctime(timeinfo),": FILE NOT FOUND AT CURRENT WORKING DIRECTORY\n"));			
			send(sock_port2,buffer,sizeof(buffer),0);
			bzero(buffer,sizeof(buffer));
			close(sock_port2);
				}

			} 
		else{
			bzero(buffer,sizeof(buffer));
			strcpy(buffer,"Please enter a valid command\n");
			send(socknew,buffer,sizeof(buffer),0);
			bzero(buffer,sizeof(buffer));

			}
		
		recv(socknew,buffer,sizeof(buffer),0);
		}

	
	if(sock_port1>0){
		close(sock_port1);
	}	
	if(sock_port2>0){
		close(sock_port2);
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
