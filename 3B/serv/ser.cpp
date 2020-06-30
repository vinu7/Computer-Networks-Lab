//#include<bits/stdc++.h>
#include<cstdio>
#include<stdlib.h>
#include<cstring>
#include<unistd.h>
#include<time.h>
#include<sys/stat.h>

#include<signal.h>


#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

using namespace std;
#define PORT1 20021
#define PORT2 80
#define PORT3 80

#define QUEUE_LIMIT 4


void get_file(char filename[], char file[])
{
	int i,j=0,count=0,temp;
	for(i=0;filename[i]!='\0' ; i++)
	{
		if(filename[i]=='/')
			count++;
	}
	//printf("counnt:%d\n",count);
	if(count==0)
	{
		for(i=0;filename[i]!='\0' ; i++)
		{
			file[i]=filename[i];
		}
		file[i]=filename[i];
	}
	else
	{
		temp=0;
		for(i=0;filename[i]!='\0' ; i++)
		{
			if(filename[i]=='/')
			{
				temp++;
				if(temp==count)
					break;
			}
		}
		i++;
		for(j=0;filename[i]!='\0';i++,j++)
		{
			file[j]=filename[i];
		}
		file[j]='\0';


	}
	//printf("file:%s\n",file);
	return;

}


void get_filename(char filename[],char relpath[],char http_version[], char buffer[],int j)
{
	//int j=4,
	int k=0;
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
		j++;
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




void getFileCreationTime(char path[],int socknew)
{
	//printf("%s\n",path);	//ctime-last status change time,mtime-modified time,atime-access time,size-filesize,mode-prot
    struct stat attr;											//ecton mode,uid,gid,ino
    stat(path, &attr);
    //printf("Last modified time: %s", ctime(&attr.st_mtime));
	char buff[100];
	//strcpy(buffer,ctime(&attr.st_mtime));
	bzero(buff,sizeof(buff));
	strftime(buff, 100, "%d-%m-%y  %H:%M:%S", localtime(&(attr.st_mtime)));//%I (12 hr form) %j(day of year) %p(Am or )
	//printf("The file %s was last modified at %s\n", path, buff);
	//printf("%u\n",attr.st_mode);
	//printf("%u\n",attr.st_uid);
	//printf("%u\n",attr.st_gid);
	
	//printf("%s",buff);
	send(socknew,buff,sizeof(buff),0);//ctime(&attr.st_mtime),sizeof(ctime(&attr.st_mtime)),0);

}


int Checkinput1(char buffer[])//,int a)//,int *b)
{
	int a=0;
	//printf("Hee\n");
	int i=0,j=0;
	for(i=0;buffer[i]!='\n';i++)
	{
		if(buffer[i]==' ')
			a++;
	}
	//printf("he2\n");
	return a;

}
int Checkinput2(char buffer[])//,int a)//,int *b)
{
	int a=0;
	//printf("Hee\n");
	int i=0,j=0;
	for(i=0;buffer[i]!='\n';i++)
	{
		if(buffer[i]==' ' && buffer[i+1]==' ')
			a++;
		
	}
	//printf("he2\n");
	return a;

}

void comm_server_to_client(int socknew)
{
	
	char buffer[100];
	int i,j;
	//int *a,*b;
	//(*a)=0;(*b)=0;
	char filename[100];
	char file[100];
	char http_version[100];
	char relpath[100];
	char temper[100];
			
	//printf("hiii\n");
	
	while(1)//strcmp(buffer,"goodbye server\n")!=0)
	{
		//break;
		//printf("%s",buffer);
		//(*a)=0;(*b)=0;
	
		//printf("hiii2\n");
		bzero(buffer,sizeof(buffer));
		recv(socknew,buffer,sizeof(buffer),0);
		printf("//S:command is %s",buffer);
		
		//printf("%s",buffer);
		int a=Checkinput1(buffer);
		int b=Checkinput2(buffer);
		//printf("%s",buffer);
		//printf("%d %d\n",a,b);
		if(((b)>0 || (a)<2 )&& strcmp(buffer,"exit\n")!=0)//bad request
		{
			bzero(temper,sizeof(temper));	
			strcpy(temper,"NOT ok\n");
			//printf("%s",temper);
			send(socknew,temper,sizeof(temper),0);


			printf("/BAD request\n");
	  		bzero(temper,sizeof(temper));
    	  	strcpy(temper,"HTTP/1.1 400"); 
      		send(socknew,temper,sizeof(temper),0);
      		bzero(temper,sizeof(temper));
		}
		else//proceed
		{
			bzero(temper,sizeof(temper));	
			strcpy(temper,"ok\n");
			//printf("%s",temper);
			send(socknew,temper,sizeof(temper),0);
		
		//get_filename(filename,relpath,http_version,buffer);
		//printf("%s\n",relpath);
		if(strncmp(buffer,"GET ",4)==0)
		{
			get_filename(filename,relpath,http_version,buffer,4);
		
			//printf("%s",buffer);
			//strcpy(filename,"test.txt");
			//printf("-----");
			
			if(strcmp(buffer,"GET / HTTP/1.1\n")==0)
			{
				//trsnfer index.html
				printf("//G:transfer index.html\n");
				bzero(buffer,sizeof(buffer));
				strcpy(buffer,"HTTP/1.1 200 ok\n");
				//printf("//>%s",buffer);
				send(socknew,buffer,sizeof(buffer),0);
//				bzero(buffer,sizeof(buffer));

				bzero(buffer,sizeof(buffer));
				FILE* fileptr = fopen("index.html","r");
				char tempfile[]="index.html";
				int fileval = file_transfer(tempfile,socknew,fileptr);
				bzero(buffer,sizeof(buffer));
					//close(sock_port1);
				fclose(fileptr);			
				
			}
			else
			{
				if(strcmp(http_version,"HTTP/1.1")==0)
				{

					//printf("//G:filename is %s\n",filename);
					//printf("//G:relpath is %s\n",relpath);

					get_file(filename,file);
					printf("file:%s\n",file);

					/*if(file[0]=='1' && file[1]=='1')
					{
						printf("Forbidden file\n");
						bzero(temper,sizeof(temper));
    		  			strcpy(temper,"HTTP/1.1 403\n"); 
	      				send(socknew,temper,sizeof(temper),0);
      					bzero(temper,sizeof(temper));


					}
					else
					{*/
					FILE* fileptr = fopen(relpath,"r");
					if(fileptr != NULL)
					{
						//printf(">12344556\n");
						//int sock_port1 = conn(PORT3,socknew);

						if(file[0]=='1' && file[1]=='1')
						{
							printf("Forbidden file\n");
							bzero(temper,sizeof(temper));
    			  			strcpy(temper,"HTTP/1.1 403\n"); 
	    	  				send(socknew,temper,sizeof(temper),0);
      						bzero(temper,sizeof(temper));

						}
						else
						{
							bzero(buffer,sizeof(buffer));
							strcpy(buffer,"HTTP/1.1 200 ok\n");
							printf("//>%s",buffer);
							send(socknew,buffer,sizeof(buffer),0);
							bzero(buffer,sizeof(buffer));

							int fileval = file_transfer(filename,socknew,fileptr);
							bzero(buffer,sizeof(buffer));
							//close(sock_port1);
						}			
						fclose(fileptr);
					}
					else
					{
						printf("//G:FILe not found\n");

						//“HTTP/1.1 404” if the file is not found
						bzero(buffer,sizeof(buffer));
						strcpy(buffer,"HTTP/1.1 404\n");
						printf("//>%s",buffer);
						send(socknew,buffer,sizeof(buffer),0);

					}
					//}

				}
				else
				{

					printf("//G:BAD request\n");
	  		 		bzero(temper,sizeof(temper));
    	  			strcpy(temper,"HTTP/1.1 400\n"); 
      				send(socknew,temper,sizeof(temper),0);
      				bzero(temper,sizeof(temper));

				}
			}
			
		}
		else if(strncmp(buffer,"HEAD ",5)==0)
		{
			get_filename(filename,relpath,http_version,buffer,5);

			if(strcmp(buffer,"HEAD / HTTP/1.1\n")==0)
			{
				bzero(buffer,sizeof(buffer));
				strcpy(buffer,"HTTP/1.1 200 ok\n");
				printf("//>%s",buffer);
				send(socknew,buffer,sizeof(buffer),0);
				char tempfile[]="index.html";
				getFileCreationTime(tempfile,socknew);
					
			}			

			else if(strcmp(http_version,"HTTP/1.1")==0)
			{
				
				get_file(filename,file);
				printf("H:head action for %s\n",relpath);	

				FILE* tempptr=fopen(relpath,"r");
				if(tempptr!=NULL)
				{

					if(file[0]=='1' && file[1]=='1')
					{
						printf("Forbidden file\n");
						bzero(temper,sizeof(temper));
    			  		strcpy(temper,"HTTP/1.1 403\n"); 
	    	  			send(socknew,temper,sizeof(temper),0);
      					bzero(temper,sizeof(temper));
					}
					else
					{
						bzero(buffer,sizeof(buffer));
						strcpy(buffer,"HTTP/1.1 200 ok\n");
						printf("//>%s",buffer);
						send(socknew,buffer,sizeof(buffer),0);
				
						getFileCreationTime(relpath,socknew);
					}
					fclose(tempptr);


				}
				else
				{
					printf("//H:FILe not found\n");

						//“HTTP/1.1 404” if the file is not found
					bzero(temper,sizeof(temper));
					strcpy(temper,"HTTP/1.1 404\n");
					printf("//>%s",temper);
					send(socknew,temper,sizeof(temper),0);

				}
			}
			else
			{

				printf("//H:BAD request\n");
  		 		bzero(temper,sizeof(temper));
   	  			strcpy(temper,"HTTP/1.1 400\n"); 
   				send(socknew,temper,sizeof(temper),0);
   				bzero(temper,sizeof(temper));


			}


		}
		else if(strncmp(buffer,"PUT ",4)==0)
		{
			printf("//P:PUT action\n");
			get_filename(filename,relpath,http_version,buffer,4);
			get_file(filename,file);
			printf("%s\n",http_version);
			printf("%s\n",file);
			
			if(strcmp(http_version,"HTTP/1.1")==0)
			{	

					bzero(buffer,sizeof(buffer));
					recv(socknew, buffer,sizeof(buffer),0);
					if(strcmp(buffer,"Ready Receive\n")==0)
					{
						
						printf("transfer started\n");
						bzero(buffer,sizeof(buffer));
						recv(socknew, buffer,sizeof(buffer),0);
						FILE* serptr=fopen(file,"w");
						while(strcmp(buffer,"-1\n")!=0)
						{
							//printf("%s",buffer);
							fprintf(serptr,"%s",buffer);
							bzero(buffer,sizeof(buffer));
							recv(socknew,buffer,sizeof(buffer),0);
						}
						bzero(buffer,sizeof(buffer));
						fclose(serptr);
						printf("File received\n");
					}
					else
					{
						printf("Transfer failed\n");
					}

			}
			else
			{
				printf("/P:BAD request\n");
  		 		bzero(temper,sizeof(temper));
      			strcpy(temper,"HTTP/1.1 400\n"); 
      			send(socknew,temper,sizeof(temper),0);
      			bzero(temper,sizeof(temper));

			}
		}




		
		else if(strncmp(buffer,"DELETE ",7)==0)
		{
			printf("DELETE action\n");
			get_filename(filename,relpath,http_version,buffer,7);
			get_file(filename,file);
			printf("%s\n",http_version);
			if((strcmp(http_version,"HTTP/1.1")==0) && (strcmp(buffer,"DELETE / HTTP/1.1\n")!=0) )
			{
				FILE* tempptr=fopen(relpath,"r");
				if (tempptr != NULL)
				{			
					fclose(tempptr);
					//get_file(filename,file);
					if(file[0]=='1' && file[1]=='1')
					{
						printf("Forbidden file\n");
						bzero(temper,sizeof(temper));
    			  		strcpy(temper,"HTTP/1.1 403\n"); 
	    	  			send(socknew,temper,sizeof(temper),0);
      					bzero(temper,sizeof(temper));
					}
					else
					{
						if(remove(relpath)==0)
						{
      						printf("Deleted %s successfully\n",filename); 
      						bzero(temper,sizeof(temper));
      						strcpy(temper,"HTTP/1.1 200 ok\n"); 
 	    	 				send(socknew,temper,sizeof(temper),0);
    	  					bzero(temper,sizeof(temper));
    	  				}
    	  				else
    	  				{
    	  					printf("//error deleting\n");
    	  					bzero(temper,sizeof(temper));
      					strcpy(temper,"HTTP/1.1 404\n"); 
      					send(socknew,temper,sizeof(temper),0);
    	  				bzero(temper,sizeof(temper));

    	  				}
    	  			}
  			 	}
  		 		else
	  		 	{
    	  			printf("//Delete Unsuccessfull\n");
      				bzero(temper,sizeof(temper));
      				strcpy(temper,"HTTP/1.1 404\n"); 
      				send(socknew,temper,sizeof(temper),0);
    	  			bzero(temper,sizeof(temper));
  			 	}
  		 	}
  		 	else
  		 	{
  		 		printf("//BAD request\n");
  		 		bzero(temper,sizeof(temper));
      			strcpy(temper,"HTTP/1.1 400\n"); 
      			send(socknew,temper,sizeof(temper),0);
      			bzero(temper,sizeof(temper));

  		 	}
  


		} 
		else if(strcmp(buffer,"exit\n")==0)
		{
			printf("exit action\n");
			break;
		}

		else
		{
			//printf("%s",buffer);
			
			printf("Enter valid method\n");
			bzero(temper,sizeof(temper));
      		strcpy(temper,"HTTP/1.1 405\n"); 
      		send(socknew,temper,sizeof(temper),0);
      		bzero(temper,sizeof(temper));



			
		}
	}
		
		
		//recv(socknew,buffer,sizeof(buffer),0);
	}
	


	//bzero(buffer,sizeof(buffer));
	//strcpy(buffer,"Thank You , All connections successfully terminated!\n");
	//send(socknew,buffer,sizeof(buffer),0);
	//printf(" im cllosing jow");
	close(socknew);
	printf("socket closed\n");
	printf("---------------------------------\n");
	return ;
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
	//int c = fork();
	int childpid=-1;
	//printf("sksk");
	if((childpid =fork()) == 0)
	{
	close(sockid);	
	comm_server_to_client(socknew);	
	break;
	}
}
close(sockid);
return 0;
}
