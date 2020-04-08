#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define MAX 200 
#define PORTMSJ 4444
#define PORTFILE 5555


int main() 
{
    bool transfer = false;
    char msjserver[MAX]; 
    char msjclient[MAX];
    char rev_file[1] = "";  
    int sockfd;
    int sockfile; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        perror("socket creation failed...\n"); 
        exit(EXIT_FAILURE); 
    }
  
    // create and assign IP, PORT 
    struct sockaddr_in * servaddrmsj = calloc(1, sizeof (struct sockaddr_in));
    servaddrmsj->sin_family = AF_INET;
    servaddrmsj->sin_port = htons(PORTMSJ);
    servaddrmsj->sin_addr.s_addr = INADDR_ANY;



    sockfile = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfile == -1) { 
        perror("socket creation failed...\n"); 
        exit(EXIT_FAILURE); 
    }
  
    // create and assign IP, PORT 
    struct sockaddr_in * servaddrfile = calloc(1, sizeof (struct sockaddr_in));
    servaddrfile->sin_family = AF_INET;
    servaddrfile->sin_port = htons(PORTFILE);
    servaddrfile->sin_addr.s_addr = INADDR_ANY;
  
    // connect the client socket to server socket 
    if (connect(sockfd, (struct sockaddr *)  servaddrmsj, sizeof (struct sockaddr)) != 0) { 
        perror("connection with the server failed...\n"); 
        exit(EXIT_FAILURE);
    } 
    while(1){
        do{
            fgets( msjserver, MAX-1, stdin );
            send(sockfd, msjserver, MAX, 0);
            bzero(msjserver, MAX); 

            recv(sockfd,msjclient, MAX, 0);
            printf("%s\n", msjclient);
            if(strstr(msjclient, "Conectar puerto de transferencia.") != NULL){
                transfer = true;
            }
            bzero(msjclient, sizeof(msjclient)); 
        }while(!transfer);
        if (connect(sockfile, (struct sockaddr *)  servaddrfile, sizeof (struct sockaddr)) != 0) { 
            perror("connection with the server failed...\n"); 
            exit(EXIT_FAILURE);
        }
        printf("CLIENTE: INCIANDO RECEPCION \n");
        bzero(rev_file, 1);
        int recibido = -1;
        FILE *image = fopen("./isoscopia/memtest86+-5.01.iso", "wb");
        long int i= 0;
        while((recibido = (int)recv(sockfile, rev_file, 1, 0)) > 0){
            // if(rev_file[0] == '|')
            //     break;
            //printf("%s",msjclient);
            fwrite(rev_file,sizeof(char),1,image);
            i++;
            if(i == 1839104)
                break;
        }
        char ola[1] = "a";
        send(sockfile, ola,1,0);
        fclose(image);
        bzero(rev_file, MAX);
        transfer = false;
        printf("CLIENTE: FINALIZADO RECEPCION \n");
    } 
  
    close(sockfd); 
} 