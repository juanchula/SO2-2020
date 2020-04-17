/**
 * @file main.c
 * @author Juan Ignacio Fernandez
 */


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
#define FOLDER "/home/juanfernandez/Facu/SO2-2020/TP1_JIFernandez_40295362/isos-cliente/"
#define PCTERMINAL "juanfernandez@Juan-Lenovo ->"

int fdsocket;

/**
 * @brief Calcula la cantidad de espacios del contenido del arreglo de char
 * @param txt Puntero de arreglo de char a procesar
 * @return Numero de espacios
 */
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

/**
 * @brief Muestra la tabla de particion de una iso
 * @param isoname Puntero de arreglo de char que contiene el nombre de la iso
 */
void partitiontable(char * isoname){
    char url[BUFF_SIZE];
    unsigned char hex[510];
    char temp[4];
    char temp4[60];
    long int size;
    int p = 4;
    size_t i = 509;
    size_t j = 0;
    size_t z;

    strcpy(url, FOLDER);
    strcat(url, isoname);
    bzero(temp4, 12);

    FILE *image = fopen(url, "rb");
    if(image == NULL){
            perror("No se a podido abrir el archivo: ");
            exit(EXIT_FAILURE);
        }

    fread(hex, 1, sizeof hex, image);
    printf("PARTICION       FIN    INICIO      TIPO        BOOTEABLE");
    while(i>445){
        while(1){
            z = i + j;
            sprintf(temp, "%02x", hex[i]);
            // printf("\n%s\n", temp);
            if(z>505 && z<510){
                strcat(temp4, (char *)temp);
                if(z == 506){
                    // printf("\nTamaño fin: %s\n", temp4);
                    size = (int) strtol(temp4, NULL, 16);
                    bzero(temp4, 60);
                    if(size == 0){
                        i-=12;
                        p--;
                        break;
                    }else{
                        printf("Particion %i:    %li", p, size);
                        p--;
                    }
                }
            }
            if(z>501 && z<506){
                strcat(temp4, (char *)temp);
                if(z == 502){
                    // printf("\nTamaño inicio: %s\n", temp4);
                    size = (int) strtol(temp4, NULL, 16);
                    bzero(temp4, 60);
                    printf("    %li", size);
                }
            }
            if(z == 498){
                printf("        %s", temp);
            }
            if(z == 494){
                size = (int) strtol(temp, NULL, 16);
                if(size == 0){
                    printf("              NO");
                } else{
                    printf("              SI");
                }
                i--;
                printf("\n");
                break;
            }
            i--;
        }
        j+=16;
    }
    fclose(image);
}

/**
 * @brief Funcion encargada de procesar la interrupcion por control+c . Envia un cierre de conexion al servidor principal
 */
void controlc(){
    char msjclient[BUFF_SIZE] ="exit\n";
    send(fdsocket, msjclient, BUFF_SIZE, 0);
    exit(0);
}

/**
 * @brief Calcula el MD5HASH de un archivo
 * @param iso Puntero al arreglo de char que contiene el nombre del archivo
 * @param md5 Puntero al arreglo de char donde se cargara el MD5
 */
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

/**
 * @brief Graba la iso en el USB
 * @param usb Puntero de arreglo de char que contiene el nombre del usb
 * @param iso Puntero de arreglo de char que contiene el nombre de la iso
 * @return 0
 */
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

/**
 * @brief Realiza la recepcion de la iso y luego muestra la tabla de particiones y verifica el MD5
 * @param msjserver Puntero de arreglo de char que contiene el tamaño y md5 de la iso entre otras cosas
 * @param msjclient Puntero de arreglo de char que contiene el nobmre del archivo entre otras cosas
 * @param sockfile File descriptor del socket de transferencia de archivo
 */
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

/**
 * @brief Funcion principal del cliente. Realiza conexiones, interactua con el servidor (envia/recibe mensajes y los procesa) y servicio de archivo (recibe archivos)
 * @return 0
 */
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
            if(send(sockfd, msjclient, BUFF_SIZE, 0) == -1){
                perror("No se ha podido enviar un mensaje al servidor: ");
                exit(EXIT_FAILURE);
            }

            if(recv(sockfd,msjserver, BUFF_SIZE, 0) == -1){
                perror("No se ha podido recibir el mensaje del servidor: ");
                exit(EXIT_FAILURE);
            }

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