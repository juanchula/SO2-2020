#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define BUFF_SIZE 200
#define BIT_SIZE 1
#define PORTMSJ 4444
#define PORTFILE 5555
#define FOLDER "./isoscopia/"


int main() 
{
    bool transfer = false;
    char url[BUFF_SIZE];
    char isoname[BUFF_SIZE];
    char msjserver[BUFF_SIZE] = ""; 
    char msjclient[BUFF_SIZE] = ""; 
    char rev_file[BIT_SIZE] = "";
    char bitchar[BUFF_SIZE] = ""; 
    long double bitdouble;
    int i = 0;
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
            bzero(msjclient, sizeof(msjclient)); 
            bzero(msjserver, sizeof(msjserver)); 
            fgets(msjclient, BUFF_SIZE-1, stdin );
            send(sockfd, msjclient, BUFF_SIZE, 0);

            recv(sockfd,msjserver, BUFF_SIZE, 0);
            printf("%s\n", msjserver);
            if(strstr(msjserver, "Conectar puerto de transferencia.") != NULL){
                transfer = true;
            } 
        }while(!transfer);

        if (connect(sockfile, (struct sockaddr *)  servaddrfile, sizeof (struct sockaddr)) != 0) { 
            perror("connection with the server failed...\n"); 
            exit(EXIT_FAILURE);
        }
        printf("CLIENTE: INCIANDO RECEPCION \n");
        bzero(rev_file, BIT_SIZE);
        bzero(isoname, BUFF_SIZE);
        bzero(url, BUFF_SIZE);
        int recibido = -1;
        sscanf(msjserver, "%*s %*s %*s %*s %*s %*s %*s %*s %s", bitchar);
        strtok(bitchar, "B");
        sscanf(msjclient, "%*s %*s %s", isoname);
        strcpy(url, FOLDER);
        strcat(url, isoname);
        printf("%s\n", url);
        bitdouble = strtod(bitchar, NULL);

        FILE *image = fopen(url, "wb");
        i = 0;
        while((recibido = (int)recv(sockfile, rev_file, BIT_SIZE, 0)) > 0){
            fwrite(rev_file,sizeof(char),BIT_SIZE,image);
            i++;
            if(i == bitdouble)
                break;
        }
        send(sockfile, "a", BIT_SIZE, 0);
        fclose(image);
        bzero(rev_file, BUFF_SIZE);
        transfer = false;
        printf("CLIENTE: FINALIZADO RECEPCION \n");
        close(sockfile);
    } 
  
    close(sockfd); 
} 