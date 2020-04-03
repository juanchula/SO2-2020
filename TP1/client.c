#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#define MAX 200 

/*
void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
    do { 
        bzero(buff, sizeof(buff)); 
        printf("Enter the string : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
        write(sockfd, buff, sizeof(buff)); 
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff)); 
        printf("From Server : %s", buff); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        }
    } while(1); 
} */

int main() 
{
    char msjserver[MAX]; 
    char msjclient[MAX];  
    int sockfd; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        perror("socket creation failed...\n"); 
        exit(EXIT_FAILURE); 
    }
  
    // create and assign IP, PORT 
    struct sockaddr_in * servaddr = calloc(1, sizeof (struct sockaddr_in));
    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(4444);
    servaddr->sin_addr.s_addr = INADDR_ANY;
  
    // connect the client socket to server socket 
    if (connect(sockfd, (struct sockaddr *)  servaddr, sizeof (struct sockaddr)) != 0) { 
        perror("connection with the server failed...\n"); 
        exit(EXIT_FAILURE);
    } 
  
    do{
        fgets( msjserver, MAX-1, stdin );
        send(sockfd, msjserver, MAX, 0);
        bzero(msjserver, MAX); 

        recv(sockfd,msjclient, MAX, 0);
        printf("%s\n", msjclient);
        bzero(msjclient, sizeof(msjclient)); 
    }while(1); 
  
    // close the socket 
    close(sockfd); 
} 