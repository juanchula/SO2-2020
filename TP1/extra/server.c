#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(void){
    //int num_port = 4444;
    char msj[200]; 
    int sfd;
    int fdc;

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

    do{
        fflush(stdout); //fuerza la descarga del buffer
        fdc = accept(sfd, (struct sockaddr *) client, (socklen_t *) &lenght_client);
        lenght_client = (int32_t) sizeof (struct sockaddr_in);
        //fread(msj, 200, 1, fdc);
        read(fdc, msj, 200);
        printf("%s\n", msj);
    }while(1);

    return 0;
}
