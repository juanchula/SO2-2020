#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#define MAX 200 

#define QUEUEPATH "/authservice" 

int main(void){
    //int num_port = 4444;
    char msjserver[MAX]; 
    char msjclient[MAX]; 
    int sfd;
    int fdc;
    bool connected = false;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) { 
        perror("socket creation failed...\n"); 
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in* server = calloc(1, sizeof (struct sockaddr_in));
    server->sin_family = AF_INET;
    server->sin_port = htons(4444);
    server->sin_addr.s_addr = INADDR_ANY;

    struct sockaddr_in* client = calloc(1, sizeof (struct sockaddr_in));
    int32_t lenght_client = (int32_t) sizeof (struct sockaddr_in);

    if(bind(sfd, (struct sockaddr *) server, sizeof (struct sockaddr)) == -1){
        perror("Error en bind: ");
        exit(EXIT_FAILURE);
    }
    
    if(listen(sfd, 1024) == -1){
        perror("Error en listen: ");
        exit(EXIT_FAILURE);
    }




    struct mq_attr queue_atributes = {0};
    queue_atributes.mq_maxmsg  = 10 ;
    queue_atributes.mq_msgsize = MAX ;

    /* permission macros(the ones starting with S_) can vi viewed in man 2 open */
    mqd_t qd = mq_open(QUEUEPATH, O_RDWR | O_CREAT, 0666 , &queue_atributes);
    if (qd == -1){
            perror("Creating queue");
            exit(EXIT_FAILURE);
    }
    char sent_msg[MAX];
    char recv_msg[MAX];
    unsigned int prio = 1;


    fflush(stdout); //fuerza la descarga del buffer
    fdc = accept(sfd, (struct sockaddr *) client, (socklen_t *) &lenght_client);
    lenght_client = (int32_t) sizeof (struct sockaddr_in);
    connected = true;
    printf("Nuevo cliente aceptado\n");
    do{
        while(connected){
            bzero(msjclient, MAX); 
            recv(fdc,msjclient, MAX, 0);
            //printf("%s\n", msjclient);
            //bzero(msjclient, MAX); 
            if(strstr(msjclient, "exit") != NULL && (int) strlen(msjclient) == 5){
                connected = false;
                strcpy(sent_msg, "logout");
                if (  mq_send(qd, sent_msg, MAX, (unsigned int) 1) == -1){
                perror("Sending");
                exit(EXIT_FAILURE);
                }
                if (mq_receive(qd, recv_msg, MAX, &prio) == -1 ){
                        perror("Receiving");
                        exit(EXIT_FAILURE);
                }
                bzero(recv_msg, MAX);
                break;
            }
            strcpy(sent_msg, msjclient);
            if (  mq_send(qd, sent_msg, MAX, (unsigned int) 1) == -1){
                perror("Sending");
                exit(EXIT_FAILURE);
            }
            bzero(sent_msg, MAX);
            
            if (mq_receive(qd, recv_msg, MAX, &prio) == -1 ){
                        perror("Receiving");
                        exit(EXIT_FAILURE);
                }
            //printf("%s\n", recv_msg);
            strcpy(msjserver, recv_msg);
            bzero(recv_msg, MAX);


            //fgets( msjserver, MAX-1, stdin );
            send(fdc, msjserver, MAX, 0);
            bzero(msjserver, MAX); 
        }
        fdc = accept(sfd, (struct sockaddr *) client, (socklen_t *) &lenght_client);
        lenght_client = (int32_t) sizeof (struct sockaddr_in);
        connected = true;
    }while(1);

    return 0;
}
