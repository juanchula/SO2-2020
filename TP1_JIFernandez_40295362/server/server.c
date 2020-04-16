/**
 * @file server.c
 * @author Juan Ignacio Fernandez
 */


#include "auth.h"
#include "fileserv.h"
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


/**
 * @brief Realiza la transferencia y recepcion de las colas de mensaje
 * @param qd File descriptor de la cola de mensaje
 * @param sent_msg Puntero al arreglo de char que contiene el mensaje a enviar
 * @param recv_msg Puntero al arreglo de char donde se guardara el mensaje recibido
 */
void sentrevcposix(int qd, char *sent_msg, char *recv_msg){
    unsigned int prio = 1;
    bzero(recv_msg, BUFF_SIZE);
    if ( mq_send(qd, sent_msg, BUFF_SIZE, (unsigned int) 1) == -1){
        perror("Sending");
        exit(EXIT_FAILURE);
    }
    if (mq_receive(qd, recv_msg, BUFF_SIZE, &prio) == -1 ){
        perror("Receiving");
        exit(EXIT_FAILURE);
    }
    bzero(sent_msg, BUFF_SIZE);
}

/**
 * @brief Funcion principal del servidor principal. Acepta conexiones, crea cola de mensajes y procesa los mensajes del cliente
 * @return 0
 */
int main(){
    __pid_t pidauth;
    __pid_t pidfile;
    char msjserver[BUFF_SIZE]; 
    char msjclient[BUFF_SIZE]; 
    int sfd;
    int fdc = -1;
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

    pidauth = fork();
    if(pidauth>0){
        pidfile = fork();
    }
    if(pidauth == 0){
        amain();
    }
    if(pidfile == 0 && pidauth>0){
       fsmain();
    }
    if(pidauth>0 && pidfile>0){
        fflush(stdout); //fuerza la descarga del buffer
        while(fdc == -1){
            fdc = accept(sfd, (struct sockaddr *) client, (socklen_t *) &lenght_client);
            if(fdc == -1){
                perror("No se ha podido aceptar la conecxion:");
            }
        }
        
        while(1){
            while(1){
                bzero(msjserver, BUFF_SIZE);
                bzero(msjclient, BUFF_SIZE); 
                if(recv(fdc,msjclient, BUFF_SIZE, 0) == -1){
                    perror("No se ha podido recibir el mensaje del cliente: ");
                    exit(EXIT_FAILURE);
                }

                if(strstr(msjclient, "exit") != NULL && (int) strlen(msjclient) == 5){      //Si recibo un exit   
                    login = false;
                    fdc = -1;
                    strcpy(sent_msg, "logout");
                    sentrevcposix(qda, sent_msg, recv_msg);
                    bzero(recv_msg, BUFF_SIZE);
                    break;
                }

                strcpy(sent_msg, msjclient);
                strtok(msjclient, " ");
                if(strstr(msjclient, "file")){                                          //Verifico para que servicio es el mensaje y si esta logueado
                    if(login){                                                          //Mensaje es para el servicio de archivos
                        sentrevcposix(qdf, sent_msg, recv_msg);
                    }else
                        strcpy(recv_msg, "Comando incorrecto");
                }else{                                                                  //Mensaje es para el servicio de autentificacion
                    sentrevcposix(qda, sent_msg, recv_msg);
                    if(strstr(recv_msg, "Se ha logueado correctamente") !=NULL){    //Verifico si se logueo
                        login = true;
                    }else
                        if(strstr(recv_msg, "Se ha cerrado sesion correctamente") !=NULL){  //Verifico si se deslogueo
                            login = false;
                        }
                }
                
                strcpy(msjserver, recv_msg);
                if(send(fdc, msjserver, BUFF_SIZE, 0) == -1){
                    perror("No se ha podido enviar un mensaje al cliente: ");
                    exit(EXIT_FAILURE);
                }
            }
            while(fdc == -1){
                fdc = accept(sfd, (struct sockaddr *) client, (socklen_t *) &lenght_client);
                if(fdc == -1){
                    perror("No se ha podido aceptar la conecxion:");
                }
            }
        }
    }
    return 0;
}
