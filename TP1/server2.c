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

#define BUFF_SIZE 1024 
#define PORT 4444
#define QUEUEPATHAUTH "/auth_service"
#define QUEUEPATHFILE "/file_service"

int main(void){
    char msjserver[BUFF_SIZE]; 
    char msjclient[BUFF_SIZE]; 
    int sfd;
    int fdc;
    bool connected = false;
    bool login = false;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) { 
        perror("socket creation failed...\n"); 
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in* server = calloc(1, sizeof (struct sockaddr_in));
    server->sin_family = AF_INET;
    server->sin_port = htons(PORT);
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
    queue_atributes.mq_msgsize = BUFF_SIZE ;
    char sent_msg[BUFF_SIZE];
    char recv_msg[BUFF_SIZE];
    unsigned int prio = 1;

    /*Cola de mensajes entre servidor primario y servicio de autentificacion*/
    mqd_t qda = mq_open(QUEUEPATHAUTH, O_RDWR | O_CREAT, 0666 , &queue_atributes);
    if (qda == -1){
            perror("Error al crear/unirse a la cola en el servidor principal: ");
            exit(EXIT_FAILURE);
    }

    /*Cola de mensajes entre servidor primario y servicio de archivos*/
    mqd_t qdf = mq_open(QUEUEPATHFILE, O_RDWR | O_CREAT, 0666 , &queue_atributes);
    if (qdf == -1){
            perror("Creating queue");
            exit(EXIT_FAILURE);
    }
  
    fflush(stdout); //fuerza la descarga del buffer
    fdc = accept(sfd, (struct sockaddr *) client, (socklen_t *) &lenght_client);
    lenght_client = (int32_t) sizeof (struct sockaddr_in);
    connected = true;
    printf("Nuevo cliente aceptado\n");

    do{
        while(connected){
            bzero(msjclient, BUFF_SIZE); 
            recv(fdc,msjclient, BUFF_SIZE, 0);

            if(strstr(msjclient, "exit") != NULL && (int) strlen(msjclient) == 5){      //Si recibo un exit   
                login = false;
                connected = false;
                strcpy(sent_msg, "logout");
                if (  mq_send(qda, sent_msg, BUFF_SIZE, (unsigned int) 1) == -1){
                perror("Sending");
                exit(EXIT_FAILURE);
                }
                if (mq_receive(qda, recv_msg, BUFF_SIZE, &prio) == -1 ){
                        perror("Receiving");
                        exit(EXIT_FAILURE);
                }
                bzero(recv_msg, BUFF_SIZE);
                break;
            }

            strcpy(sent_msg, msjclient);
            strtok(msjclient, " ");
            if(strstr(msjclient, "file")){                                          //Verifico para que servicio es el mensaje y si esta logueado
                if(login){                                                          //Mensaje es para el servicio de archivos
                    if (  mq_send(qdf, sent_msg, BUFF_SIZE, (unsigned int) 1) == -1){
                        perror("Sending");
                        exit(EXIT_FAILURE);
                    }
                    bzero(sent_msg, BUFF_SIZE);
                    
                    if (mq_receive(qdf, recv_msg, BUFF_SIZE, &prio) == -1 ){
                                perror("Receiving");
                                exit(EXIT_FAILURE);
                        }
                }else
                    strcpy(recv_msg, "Comando incorrecto");
            }else{                                                                  //Mensaje es para el servicio de autentificacion
                if (  mq_send(qda, sent_msg, BUFF_SIZE, (unsigned int) 1) == -1){
                    perror("Sending");
                    exit(EXIT_FAILURE);
                }
                bzero(sent_msg, BUFF_SIZE);
                
                if (mq_receive(qda, recv_msg, BUFF_SIZE, &prio) == -1 ){
                            perror("Receiving");
                            exit(EXIT_FAILURE);
                    }

                if(strstr(recv_msg, "Se ha logueado correctamente") !=NULL){    //Verifico si se logueo
                    login = true;
                }else
                    if(strstr(recv_msg, "Se ha cerrado sesion correctamente") !=NULL){  //Verifico si se deslogueo
                        login = false;
                    }
            }
            
            strcpy(msjserver, recv_msg);
            bzero(recv_msg, BUFF_SIZE);

            send(fdc, msjserver, BUFF_SIZE, 0);
            bzero(msjserver, BUFF_SIZE); 
        }
        fdc = accept(sfd, (struct sockaddr *) client, (socklen_t *) &lenght_client);
        lenght_client = (int32_t) sizeof (struct sockaddr_in);
        connected = true;
        printf("Nuevo cliente aceptado\n");
    }while(1);

    return 0;
}
