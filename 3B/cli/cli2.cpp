#include<cstdio>
#include<stdlib.h>
#include<unistd.h>
#include<cstring>

#include<time.h>
#include<sys/stat.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define PORT1 20021
#define PORT2 20021
#define PORT3 20021


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


void get_filename(char filename[],char relpath[],char http_version[], char buffer[],int j)
{
	//int j=4,
//	printf("here\n");
	int k=0;
	int i =0;
	relpath[0]='.';
	relpath[1]='/';
	for(i = 0 ; (buffer[j]!=' ' && buffer[j]!='\n') ;i++,j++)
	{
		filename[i] = buffer[j];
		relpath[i+2]=buffer[j];
	}
	if(buffer[j]==' ')
	{
		j++;
		for(k=0;buffer[j]!='\n';k++,j++)
		{
			http_version[k]=buffer[j];
		}
	}

	filename[i]='\0';
	relpath[i+2]='\0';
	http_version[k]='\0';
//printf("here2\n");

	return ;
}





void comm_clienttoserver(int sockid)
{
	char buffer[100];//={'\0'};
	char filename[100];
	char http_version[100];
	char relpath[100];
	char temp3[100];
	
	while(1)//strcmp(buffer,"goodbye server\n")!=0){
	{
		bzero(buffer,sizeof(buffer));
		char *c= fgets(buffer,sizeof(buffer),stdin);
		
		printf("//Command is:%s",buffer);
		send(sockid,buffer,sizeof(buffer),0);
			
			//printf("sent\n");
		bzero(temp3,sizeof(temp3));
		recv(sockid, temp3,sizeof(temp3),0);
		//printf("%s",temp3);
		if(strcmp(temp3,"NOT ok\n")==0)
		{
			bzero(temp3,sizeof(temp3));
			printf("//G:Bad request/n");
			recv(sockid,temp3,sizeof(temp3),0);
			printf("%s\n",temp3 );

		}
		else
		{


		if(strncmp(buffer,"GET ",4)==0)
		{
			if(strcmp(buffer,"GET / HTTP/1.1\n")==0)
			{
				//default index.html
				bzero(buffer,sizeof(buffer));
				recv(sockid, buffer,sizeof(buffer),0);
				printf(">%s\n",buffer);
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

				//get_filename(filename,relpath,http_version,buffer,4);
					
				if(1==1)//strcmp(http_version,"HTTP/1.1")==0)
				{
					//printf("%s\n",filename);
					//printf("%s\n",http_version);

					//printf("relpath is %s\n",relpath);

					bzero(buffer,sizeof(buffer));
					recv(sockid,buffer,sizeof(buffer),0);

					if(strcmp(buffer,"HTTP/1.1 403\n")==0)
					{
						printf("//>Forbidden file\n");
						//recv(sock_port2,buffer,sizeof(buffer),0);
						printf("%s",buffer);
						bzero(buffer,sizeof(buffer));
					}
					else if(strcmp(buffer,"HTTP/1.1 200 ok\n")==0)//strcmp(buffer,"conn 20020\n")==0)
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
					else if(strcmp(buffer,"HTTP/1.1 404\n")==0)//strcmp(buffer,"conn 20023\n")==0)
					{
						printf("//>File not found\n");
						//recv(sock_port2,buffer,sizeof(buffer),0);
						printf("%s",buffer);
						bzero(buffer,sizeof(buffer));
			
					}
					else if(strcmp(buffer,"HTTP/1.1 400\n")==0)
					{
						//bzero(temp3,sizeof(temp3));
						printf("//>G:Bad request\n");
						//recv(sockid,temp3,sizeof(temp3),0);
						printf("%s\n",buffer );
						bzero(buffer,sizeof(buffer));
					

					}
					else
					{
						//
						printf("do some action\n");
					}
				}
				/*else
				{
					bzero(temp3,sizeof(temp3));
					printf("//>G:Bad request\n");
					recv(sockid,temp3,sizeof(temp3),0);
					printf("%s\n",temp3 );
					bzero(temp3,sizeof(temp3));
					

				}*/


			}
			
			
		}
		else if(strncmp(buffer,"HEAD ",5)==0)
		{
				//printf("head action\n");
				//get_filename(filename,relpath,http_version,buffer,5);
				//bzero(buffer,sizeof(buffer));
					bzero(temp3,sizeof(temp3));
						
				recv(sockid,temp3,sizeof(temp3),0);

				if(strcmp(temp3,"HTTP/1.1 403\n")==0)
				{
					printf("//H:file Forbidden\n");
					printf("%s",temp3);

				}
				else if(strcmp(temp3,"HTTP/1.1 404\n")==0)
				{
					printf("//H:file not found\n");
					printf("%s",temp3);

				}
				else if(strcmp(temp3,"HTTP/1.1 400\n")==0)
				{
					//bzero(temp3,sizeof(temp3));
					printf("//H:Bad request/n");
					//recv(sockid,temp3,sizeof(temp3),0);
					printf("%s\n",temp3 );

				}
				else if(strcmp(temp3,"HTTP/1.1 200 ok\n")==0)
				{
					printf("%s",temp3);
					bzero(temp3,sizeof(temp3));
					recv(sockid,temp3,sizeof(temp3),0);
					//printf("%s\n",temp3 );

					printf(">Last modified time is %s\n",temp3);
					bzero(temp3,sizeof(temp3));
					
				}
				else
				{
					printf("%s",temp3);
					printf("hlkiri\n");
				}
			//bzero(buf,sizeof(buffer));
			
			

		}
		else if(strncmp(buffer,"PUT ",4)==0)
		{
			//printf("PUT action\n");
			get_filename(filename,relpath,http_version,buffer,4);
			//printf("%s\n",http_version);
			if(strcmp(http_version,"HTTP/1.1")==0)
			{	
				//printf("%s and relpath is %s\n",filename,relpath);
				FILE* cliptr=fopen(relpath,"r");
				if(cliptr!=NULL)
				{
					bzero(temp3,sizeof(temp3));
					strcpy(temp3,"Ready Receive\n");
					//printf("%s",temp3);
					send(sockid,temp3,sizeof(temp3),0);

					int fileval = file_transfer(filename,sockid,cliptr);
					bzero(buffer,sizeof(buffer));
					//close(sock_port1);
					fclose(cliptr);			
				
				}
				else
				{
					bzero(temp3,sizeof(temp3));
					strcpy(temp3,"Not REady\n");
					//printf("%s",temp3);
					send(sockid,temp3,sizeof(temp3),0);


					printf("//File not present\n");
					//fclose(cliptr);
				}
			}
			else
			{
					bzero(temp3,sizeof(temp3));
					printf("//G:Bad request\n");
					recv(sockid,temp3,sizeof(temp3),0);
					printf("%s\n",temp3 );

			}




		}
		else if(strncmp(buffer,"DELETE ",7)==0)
		{
			//printf("DELETE action\n");
			bzero(temp3,sizeof(temp3));
			recv(sockid,temp3,sizeof(temp3),0);
			if(strcmp(temp3,"HTTP/1.1 200 ok\n")==0)
			{
				printf("//Deleted successfully\n");
				printf("%s\n",temp3);
			}
			else if(strcmp(temp3,"HTTP/1.1 404\n")==0)
			{
				printf("//Delete unsuccessfull\n");
				printf("%s\n",temp3);	
			}
			else if(strcmp(temp3,"HTTP/1.1 400\n")==0)
			{
				printf("//D:Bad request\n");
				printf("%s\n",temp3 );
			}
			else if(strcmp(temp3,"HTTP/1.1 403\n")==0)
			{
				printf("//D:Forbidden\n");
				printf("%s\n",temp3 );
			}
			else
			{
				printf("hoo-hoo\n");
			}

		} 
		else if(strcmp(buffer,"exit\n")==0)
		{
			
			printf("exit action\n");
			break;
		}

		else
		{
			printf("//Enter valid method\n");
			bzero(temp3,sizeof(temp3));
			recv(sockid,temp3,sizeof(temp3),0);
			printf("%s\n",temp3);

		}
	}

	}
		
	printf("---------------------------------\n");
	
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
	comm_clienttoserver(sockid);
	
	close(sockid);
return 0;
}

