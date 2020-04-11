#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/md5.h>
#include <signal.h>

#define BUFF_SIZE 200
#define BIT_SIZE 1
#define PORTMSJ 4444
#define PORTFILE 5555
#define FOLDER "./isoscopia/"
#define PCTERMINAL "juanfernandez@Juan-Lenovo ->"

int fdsocket;

void partitiontable(char * isoname){
    char url[BUFF_SIZE];
    strcpy(url, FOLDER);
    strcat(url, isoname);
    unsigned char hex[510]; 
    FILE *image = fopen(url, "rb");
    if(image == NULL){
            perror("No se a podido abrir el archivo: ");
            exit(EXIT_FAILURE);
        }

    fread(hex, 1, sizeof hex, image);
    for(size_t j = 0; j < (sizeof(hex)) ; j++) {
        if(j>=446){
            printf("%02x ", hex[j]);
        }
        if(j==461 || j==477 || j==493){
            printf("\n");
        }
    }
    fclose(image);
}

void controlc(){
    char msjclient[BUFF_SIZE] ="exit\n";
    send(fdsocket, msjclient, BUFF_SIZE, 0);
    exit(0);
}

void calcmd5(char *iso, char *md5){
    bzero(md5, BUFF_SIZE);
    char aux[2] = "";
    char url[BUFF_SIZE] = FOLDER;
    strcat(url, iso);
    //OTROS LINKS: https://stackoverflow.com/questions/1220046/how-to-get-the-md5-hash-of-a-file-in-c 
    //https://stackoverflow.com/questions/3395690/md5sum-of-file-in-linux-c
    //https://stackoverflow.com/questions/7627723/how-to-create-a-md5-hash-of-a-string-in-c
    //SACADO DE: https://stackoverflow.com/questions/10324611/how-to-calculate-the-md5-hash-of-a-large-file-in-c
    unsigned char c[MD5_DIGEST_LENGTH];
    FILE *inFile = fopen (url, "rb");
    MD5_CTX mdContext;
    size_t bytes;
    unsigned char data[BUFF_SIZE];

    if (inFile == NULL) {
        perror("No se ha podido abrir la isos: ");
        exit (EXIT_FAILURE);
    }

    MD5_Init (&mdContext);
    while ((bytes = fread(data, 1, BUFF_SIZE, inFile))){
        MD5_Update (&mdContext, data, bytes);
    }

    MD5_Final (c,&mdContext);
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        snprintf(aux, (MD5_DIGEST_LENGTH*2), "%02x", c[i]); //Importante el x2
        strcat(md5, aux);
        //printf("%02x", c[i]);
    }
    fclose(inFile);
}

void receivefile(char* msjserver, char *msjclient, int sockfile){
    char buffer[BUFF_SIZE];
    ssize_t receivedbyte;
    double missingbytes;
    char filesize[BUFF_SIZE];
    char originalmd5[BUFF_SIZE];
    char localmd5[BUFF_SIZE];
    char url[BUFF_SIZE];
    char isoname[BUFF_SIZE];


    sscanf(msjserver, "%*s %*s %*s %*s %*s %*s %*s %*s %s %s", filesize, originalmd5);
    sscanf(msjclient, "%*s %*s %s", isoname);
    strtok(filesize, "B");
    strcpy(url, FOLDER);
    strcat(url, isoname);

    missingbytes = atoi(filesize);

    FILE *image = fopen(url, "wb");
    if(image == NULL){
        perror("No se a podido abrir el archivo: ");
        exit(EXIT_FAILURE);
    }

    while (((receivedbyte = recv(sockfile, buffer, BUFF_SIZE, 0)) > 0) && (missingbytes > 0)){
        fwrite(buffer, sizeof(char), (size_t) receivedbyte, image);
        missingbytes -= (int) receivedbyte;
    }
    fclose(image);

    calcmd5(isoname, localmd5);
    if(strstr(localmd5, originalmd5) != NULL){
        printf("CLIENTE: FINALIZADA RECEPCION CON EXITO. MD5:%s\n Tabla de particion:\n", localmd5);
        partitiontable(isoname);
        
    }else{
            printf("CLIENTE: FINALIZADA RECEPCION CON ERROR. MD5:%s\n No se grabara en el usb", localmd5);
    }
    close(sockfile);
}


int main() 
{
    bool transfer = false;
    // char url[BUFF_SIZE];
    // char isoname[BUFF_SIZE];
    char msjserver[BUFF_SIZE] = ""; 
    char msjclient[BUFF_SIZE] = ""; 
    // char rev_file[BIT_SIZE] = "";
    // char bitchar[BUFF_SIZE] = "";
    // char md5original[BUFF_SIZE] = ""; 
    // char md5local[BUFF_SIZE] = "";
    // long double bitdouble;
    // int i = 0;
    int sockfd;
    int sockfile; 

    struct sigaction sa;
    sa.sa_handler = controlc;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    if(sigaction(SIGINT, &sa, NULL) == -1){
        perror("Error signaction: ");
        exit(EXIT_FAILURE);
    }

    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        perror("socket creation failed...\n"); 
        exit(EXIT_FAILURE); 
    }
    fdsocket = sockfd;
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
            printf("%s", PCTERMINAL);
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
        receivefile(msjserver, msjclient, sockfile);
        transfer = false;
        // bzero(rev_file, BIT_SIZE);
        // bzero(isoname, BUFF_SIZE);
        // bzero(url, BUFF_SIZE);
        // bzero(md5original, BUFF_SIZE);
        // bzero(md5local, BUFF_SIZE);
        // int recibido = -1;
        // sscanf(msjserver, "%*s %*s %*s %*s %*s %*s %*s %*s %s %s", bitchar, md5original);
        // strtok(md5original, "\n");
        // strtok(bitchar, "B");
        // sscanf(msjclient, "%*s %*s %s", isoname);
        // strcpy(url, FOLDER);
        // strcat(url, isoname);
        // printf("%s\n", url);
        // bitdouble = strtod(bitchar, NULL);

        // FILE *image = fopen(url, "wb");
        // if(image == NULL){
        //     perror("No se a podido abrir el archivo: ");
        //     exit(EXIT_FAILURE);
        // }

        // i = 0;
        // while((recibido = (int)recv(sockfile, rev_file, BIT_SIZE, 0)) > 0){
        //     fwrite(rev_file,sizeof(char),BIT_SIZE,image);
        //     i++;
        //     if(i == bitdouble)
        //         break;
        // }
        // send(sockfile, "a", BIT_SIZE, 0);
        // fclose(image);
        // bzero(rev_file, BUFF_SIZE);
        // transfer = false;
        // calcmd5(isoname, md5local);
        // if(strstr(md5local, md5original) != NULL){
        //     printf("CLIENTE: FINALIZADA RECEPCION CON EXITO. MD5:%s\n Tabla de particion:\n", md5local);
        //     partitiontable(isoname);
            
        // }else{
        //      printf("CLIENTE: FINALIZADA RECEPCION CON ERROR. MD5:%s\n No se grabara en el usb", md5local);
        // }
        // close(sockfile);
    
    }
    close(sockfd); 
} 