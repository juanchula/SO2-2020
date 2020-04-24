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
#define PORTMSJ 4444
#define PORTFILE 5555
#define PCTERMINAL "\033[1m\033[37mUsuario@PC-Usuario -> \033[0m"

int fdsocket;

 /**
 * @brief Verifica si 2 arreglo de char contienen lo mismo
 * @param length Longitud de la frase/palabra a comparar
 * @param txt Puntero de arreglo de char a comparar
 * @param searchtxt Puntero de arreglo  de char a comparar
 * @return True si los contenidos son iguales y false si no
 */
bool comparetxt(int length, char *txt, char *searchtxt){
    if(length == (int) strlen(txt) && strstr(txt, searchtxt) != NULL){
        return true;
    }else{
        return false;
    }
}

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
 * @brief Muestra en pantalla una cantidad de espacios que depende de la longitud de num
 * @param num Numero que se usara para calcular la cantiad de espacios
 */
void printspace(long int num){
    int i = 17;
    int sizenum = 0;

    do{
        sizenum++;
        num /= 10;
    }while(num != 0);
    i -= sizenum;
    while(i >= 0){
        printf(" ");
        i--;
    }
}

/**
 * @brief Muestra la tabla de particion de una iso
 * @param usb Puntero de arreglo de char que contiene el nombre del usb
 * @return 1 si se completo con exito, 0 si fallo
 */
int partitiontable(char * usb){
    char url[BUFF_SIZE];
    unsigned char hex[510];
    char temp[4];
    char typep[4];
    char temp4[60];
    long int sizep = 0;
    long int startp;
    long int endp;
    int bootp;
    int p = 4;
    size_t i = 509;
    size_t j = 0;
    size_t z;

    strcpy(url, "/dev/");
    strcat(url, usb);
    bzero(temp4, 12);

    FILE *image = fopen(url, "rb");
    if(image == NULL){
            perror("No se a podido abrir el archivo: ");
            return 0;
        }

    if(fread(hex, 1, sizeof hex, image) == 0){
        perror("No se ha podido obtener la tabla de particion: ");
        return 0;
    }
    printf("PARTICION       BOOTEABLE           COMIENZO          FINAL             SECTORES          ID\n");
    while(i>445){
        while(1){
            z = i + j;
            sprintf(temp, "%02x", hex[i]);
            if(z>505 && z<510){
                strcat(temp4, temp);
                if(z == 506){
                    sizep = (int) strtol(temp4, NULL, 16);
                    bzero(temp4, 60);
                    if(sizep == 0){
                        i-=12;
                        p--;
                        break;
                    }else{
                        printf("Particion %i:", p);
                        p--;
                    }
                }
            }
            if(z>501 && z<506){
                strcat(temp4, temp);
                if(z == 502){
                    startp = (int) strtol(temp4, NULL, 16);
                    bzero(temp4, 60);
                    endp = startp + sizep;
                }
            }
            if(z == 498){
                strcpy(typep, temp);
            }
            if(z == 494){
                bootp = (int) strtol(temp, NULL, 16);
                if(bootp == 0){
                    printf("        No              ");
                } else{
                    printf("        SI              ");
                }
                i--;
                printf("%li", startp);
                printspace(startp);
                printf("%li", endp);
                printspace(endp);
                printf("%li", sizep);
                printspace(sizep);
                printf("%s", typep);
                printf("\n");
                break;
            }
            i--;
        }
        j+=16;
    }
    fclose(image);
    return 1;
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
 * @param usb Puntero al arreglo de char que contiene el nombre del usb
 * @param md5 Puntero al arreglo de char donde se cargara el MD5
 * @param totalbytes tamaño del archivo
 * @return 1 si se completo con exito, 0 si fallo
 */
int calcmd5(char *usb, char *md5, double totalbytes){
    bzero(md5, BUFF_SIZE);
    char aux[32] = "";
    char url[BUFF_SIZE] = "/dev/";
    strcat(url, usb);
    unsigned char c[MD5_DIGEST_LENGTH];
    FILE *inFile = fopen (url, "rb");
    MD5_CTX mdContext;
    size_t bytes;
    unsigned char data[BUFF_SIZE];

    if (inFile == NULL) {
        perror("No se ha podido abrir la isos: ");
        return 0;
    }

    MD5_Init (&mdContext);
    while(totalbytes >= BUFF_SIZE){
        bytes = fread(data, 1, BUFF_SIZE, inFile);
        if(bytes == 0){
            perror("Error al calcular el MD5: ");
            fclose(inFile);
            return 0;
        }
        MD5_Update (&mdContext, data, bytes);
        totalbytes -= (double) bytes;
    }
    if(totalbytes > 0){
         bytes = fread(data, 1, (size_t) totalbytes, inFile);
          MD5_Update (&mdContext, data, bytes);
    }

    MD5_Final (c,&mdContext);
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        snprintf(aux, (MD5_DIGEST_LENGTH*2), "%02x", c[i]);
        strcat(md5, aux);
    }
    fclose(inFile);
    return 1;
}

/**
 * @brief Realiza la recepcion de la iso y luego muestra la tabla de particiones y verifica el MD5
 * @param msjserver Puntero de arreglo de char que contiene el tamaño y md5 de la iso entre otras cosas
 * @param msjclient Puntero de arreglo de char que contiene el nobmre del usb
 */
void receivefile(char* msjserver, char *msjclient){
    char buffer[BUFF_SIZE];
    ssize_t receivedbyte;
    double missingbytes;
    double totalbytes;
    char filesize[BUFF_SIZE];
    char originalmd5[BUFF_SIZE];
    char localmd5[BUFF_SIZE];
    char url[BUFF_SIZE];
    char isoname[BUFF_SIZE];
    char usbname[BUFF_SIZE];
    int sockfile;

    if(amountspace(msjclient) == 3){
        sscanf(msjclient, "%*s %*s %*s %s", usbname);
    }else{
        printf("No ingreso el usb");
        exit(EXIT_FAILURE); 
    }
    
    sockfile = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfile == -1) { 
        perror("No se ha podido crear el socket: "); 
        exit(EXIT_FAILURE); 
    }
    struct sockaddr_in * servaddrfile = calloc(1, sizeof (struct sockaddr_in));
    servaddrfile->sin_family = AF_INET;
    servaddrfile->sin_port = htons(PORTFILE);
    servaddrfile->sin_addr.s_addr = INADDR_ANY;
    if (connect(sockfile, (struct sockaddr *)  servaddrfile, sizeof (struct sockaddr)) != 0) { 
        perror("Conexion con el servicio de archivos incorrecta: "); 
        exit(EXIT_FAILURE);
    }

    sscanf(msjserver, "%*s %*s %*s %*s %*s %*s %*s %*s %s %s", filesize, originalmd5);
    sscanf(msjclient, "%*s %*s %s", isoname);
    strtok(filesize, "B");
    strcpy(url, "/dev/");
    strcat(url, usbname);

    missingbytes = atoi(filesize);
    totalbytes = missingbytes;

    FILE *image = fopen(url, "w+b");
    if(image == NULL){
        perror("No se a podido abrir el archivo: ");
        exit(EXIT_FAILURE);
    }
    while (1){
        receivedbyte = recv(sockfile, buffer, BUFF_SIZE, 0);
        if(receivedbyte == -1){
            perror("Error en la recepcion de la iso: ");
            exit(EXIT_FAILURE);
        }
        fwrite(buffer, sizeof(char), (size_t) receivedbyte, image);
        missingbytes -= (int) receivedbyte;
         if(missingbytes == 0){
             break;
         }
    }
    fclose(image);

    if((calcmd5(usbname, localmd5, totalbytes)) == 1){
        if(strstr(localmd5, originalmd5) != NULL){
            printf("Transferencia finalizada correctamente. MD5:%s\nTabla de particion:\n", localmd5);
            partitiontable(usbname);
        }else{
            printf("Transferencia finalizada con errores. MD5 obtenido:%s - MD5 original:%s\n", localmd5, originalmd5);
        }
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
            while(fgets(msjclient, BUFF_SIZE-1, stdin ) == 0);
            if(send(sockfd, msjclient, BUFF_SIZE, 0) == -1){
                perror("No se ha podido enviar un mensaje al servidor: ");
                exit(EXIT_FAILURE);
            }
            if(comparetxt(5, msjclient, "exit")){
                return 0;
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
        printf("Transferencia iniciada\n");
        receivefile(msjserver, msjclient);
    }
    close(sockfd); 
} 