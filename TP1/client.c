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

#define BUFF_SIZE 1024
#define BIT_SIZE 1
#define PORTMSJ 4444
#define PORTFILE 5555
#define FOLDER "./isoscopia/"
#define PCTERMINAL "juanfernandez@Juan-Lenovo ->"

int fdsocket;

int amountspace(char * txt){
    int space = 0;
    //int length = sizeof(txt);
    int length =(int) strlen(txt);
    for (int i = 0; i < length; i++){
        if(txt[i] == ' '){
            space++;
        }
    }
    return space;
}

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
    printf("\n");
    fclose(image);
}

void controlc(){
    char msjclient[BUFF_SIZE] ="exit\n";
    send(fdsocket, msjclient, BUFF_SIZE, 0);
    exit(0);
}

//SACADO DE: https://stackoverflow.com/questions/10324611/how-to-calculate-the-md5-hash-of-a-large-file-in-c
void calcmd5(char *iso, char *md5){
    bzero(md5, BUFF_SIZE);
    char aux[2] = "";
    char url[BUFF_SIZE] = FOLDER;
    strcat(url, iso);
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

int burniso(char *usb, char *iso){
    if(strstr(usb, "sda") !=NULL){
        printf("Esta intentando escribir el disco\n");
        exit(EXIT_FAILURE);
    }

    char i[1] = "";
    char urlusb[BUFF_SIZE];
    char urliso[BUFF_SIZE];

    strcpy(urlusb, "/dev/");
    strcat(urlusb, usb);
    strcpy(urliso, FOLDER);
    strcat(urliso, iso);
    printf("%s\n%s\n", urliso, urlusb);
    FILE *original = fopen(urliso, "rb");
    FILE *copia = fopen(urlusb, "wb");
    if(original == NULL){
        perror("No se ha podido abrir el archivo: ");
        fclose(original);
        fclose(copia);
        return 0;
    }
    if(copia == NULL){
        perror("No se ha podido acceder al usb: ");
        fclose(original);
        fclose(copia);
        return 0;
    }

    while(!feof(original)){
        fread(i, sizeof(char), 1, original);
        fwrite(i, sizeof(char), 1, copia);
    }
    fclose(original);
    fclose(copia);
    printf("USB listo\n");
    return 0;
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
    char usbname[BUFF_SIZE];

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
        printf("Transferencia finalizada correctamente. md5:%s\nTabla de particion:\n", localmd5);
        partitiontable(isoname);

        if(amountspace(msjclient) == 3){
            sscanf(msjclient, "%*s %*s %*s %s", usbname);
            burniso(usbname, isoname);
        }   
    }else{
        printf("Transferencia finalizada con errores. md5 obtenido::%s - md5 original%s\n No se procedera a la grabacion del usb", localmd5, originalmd5);
    }
    close(sockfile);
}


int main(){
    char msjserver[BUFF_SIZE]; 
    char msjclient[BUFF_SIZE]; 
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
        while(1){
            bzero(msjclient, sizeof(msjclient)); 
            bzero(msjserver, sizeof(msjserver)); 
            printf("%s", PCTERMINAL);
            fgets(msjclient, BUFF_SIZE-1, stdin );
            send(sockfd, msjclient, BUFF_SIZE, 0);

            recv(sockfd,msjserver, BUFF_SIZE, 0);
            if(strstr(msjserver, "Conectar puerto de transferencia.") != NULL){
                break;
            }
            printf("%s\n", msjserver);
        }
        if (connect(sockfile, (struct sockaddr *)  servaddrfile, sizeof (struct sockaddr)) != 0) { 
            perror("connection with the server failed...\n"); 
            exit(EXIT_FAILURE);
        }
        printf("Transferencia iniciada\n");
        receivefile(msjserver, msjclient, sockfile);
    }
    close(sockfd); 
} 