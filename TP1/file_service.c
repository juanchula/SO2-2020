#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <openssl/md5.h>
#include <mqueue.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define QUEUEPATH "/file_service"
#define BUFF_SIZE 200
#define DIRECTORY "./isos/"
#define PORT 5555

int amountspace(char * txt){
    int space = 0;
    int length =(int) strlen(txt);
    for (int i = 0; i < length; i++){
        if(txt[i] == ' '){
            space++;
        }
    }
    return space;
}

bool comparetxt(int length, char *txt, char *searchtxt){
    if(length == (int) strlen(txt) && strstr(txt, searchtxt) != NULL){
        return true;
    }else{
        return false;
    }
}

void calcmd5(char *iso, char *md5){
    bzero(md5, BUFF_SIZE);
    char aux[2] = "";
    char url[BUFF_SIZE] = DIRECTORY;
    strcat(url, iso);
    //printf("%s", url);
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

long int filesize(char *iso){
    int long size;
    char url[BUFF_SIZE] = DIRECTORY;
    strcat(url, iso);

    FILE *image = fopen(url, "r");
    if (image == NULL) {
        perror("No se ha podido abrir la isos: ");
        exit (EXIT_FAILURE);
    }
   
    if ( fseek(image, 0L, SEEK_END) == -1) {
        perror("No se ha ir al final de la iso: ");
        exit (EXIT_FAILURE);
    }

    size = ftell(image);
    if ( size == -1) {
        perror("No se ha podido obtener el tamaño de la iso: ");
        exit (EXIT_FAILURE);
    }
    fclose(image);
    return size;
}

void lsfile(char *txt){
    bzero(txt, BUFF_SIZE);
    int long s;
    char size[BUFF_SIZE] = "";
    char iso[BUFF_SIZE] = "";
    char md5[BUFF_SIZE] = "";
    DIR *d = opendir("./isos");
    struct dirent *dentry;
    
    if(d != NULL){
        while((dentry=readdir(d)) != NULL){
            strcpy(iso, dentry->d_name);
            if(strstr(iso, ".iso") !=NULL){
                calcmd5(iso, md5);
                s = filesize(iso);
                sprintf(size, "%ld", s);
                strcat(txt, iso);
                strcat(txt, "   ");
                strcat(txt, size);
                strcat(txt, "B   ");
                strcat(txt, md5);
                strcat(txt, "\n");
            }
        }
        closedir(d);
    }else{
        perror("No se ha podido abrir el directorio: ");
        exit (EXIT_FAILURE);
    }
}

bool fileverification(char *filename, char *txt){
    bzero(txt, BUFF_SIZE);
    int long s;
    char size[BUFF_SIZE] = "";
    char md5[BUFF_SIZE] = "";
    char iso[BUFF_SIZE] = "";
    DIR *d = opendir("./isos");
    struct dirent *dentry;
    
    if(d != NULL){
        while((dentry=readdir(d)) != NULL){
            strcpy(iso, dentry->d_name);
            if(comparetxt((int) strlen(filename), iso, filename)){
                if(strstr(iso, ".iso") !=NULL){
                calcmd5(iso, md5);
                s = filesize(iso);
                sprintf(size, "%ld", s);
                strcat(txt, iso);
                strcat(txt, "   ");
                strcat(txt, size);
                strcat(txt, "B   ");
                strcat(txt, md5);
                // strcat(txt, "\n");
                return true;
                }
            }
        }
        closedir(d);
    }else{
        perror("No se ha podido abrir el directorio: ");
        exit (EXIT_FAILURE);
    }
    return false;
}

int main(){
    bool transfer = false;
    int space;
    char argone[BUFF_SIZE] = "";
    char argtwo[BUFF_SIZE] = "";
    char argthree[BUFF_SIZE] = "";
    char sent_msg[BUFF_SIZE] = "";
    char recv_msg[BUFF_SIZE] = "";
    char filetransfer[BUFF_SIZE] = "";
    char url[BUFF_SIZE] = "";


    int sfd;
    int fdc;

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



    mqd_t qd = mq_open(QUEUEPATH, O_RDWR);
    if (qd == -1){
            perror("Error al abrir la cola en el servicio de autentificacion: ");
            exit(EXIT_FAILURE);
    }
    unsigned int prio = 1;
    while(1){
        do{
            bzero(recv_msg, BUFF_SIZE);
            bzero(sent_msg, BUFF_SIZE);
            if (mq_receive(qd, recv_msg, BUFF_SIZE, &prio) == -1 ){
                        perror("Error al recibir mensaje en el servicio de autentificacion: ");
                        exit(EXIT_FAILURE);
                }

            strtok(recv_msg, "\n");
            space = amountspace(recv_msg);
            switch (space){
            case 1: sscanf(recv_msg, "%s %s", argone, argtwo);
                    if(comparetxt(4, argone, "file") && comparetxt(2, argtwo, "ls")){
                        lsfile(sent_msg);
                    } else{
                        strcpy(sent_msg, "Comando incorrecto");
                    }
                    bzero(argone, BUFF_SIZE);
                    bzero(argtwo, BUFF_SIZE);
                    break;

            case 2: sscanf(recv_msg, "%s %s %s", argone, argtwo, argthree);
                    if(comparetxt(4, argone, "file") && comparetxt(4, argtwo, "down") && fileverification(argthree, filetransfer)){
                        transfer = true;
                        bzero(argone, BUFF_SIZE);
                        bzero(argtwo, BUFF_SIZE);
                        strcpy(sent_msg, "Conectar puerto de transferencia. Datos de iso: ");
                        strcat(sent_msg, filetransfer);
                    } else{
                        strcpy(sent_msg, "Comando incorrecto");
                        bzero(argone, BUFF_SIZE);
                        bzero(argtwo, BUFF_SIZE);
                        bzero(argthree, BUFF_SIZE);
                    }
                break;
            
            default: strcpy(sent_msg, "Comando incorrecto");
                break;
            }

            if (  mq_send(qd, sent_msg, BUFF_SIZE, (unsigned int) 1) == -1){
                perror("Error en envio de mensaje de servicio de autentificacion: ");
                exit(EXIT_FAILURE);
            }
        }while(!transfer);
        fdc = accept(sfd, (struct sockaddr *) client, (socklen_t *) &lenght_client);
        lenght_client = (int32_t) sizeof (struct sockaddr_in);
        bzero(url, BUFF_SIZE);
        strcpy(url, DIRECTORY);
        strcat(url, argthree);

        FILE *image = fopen(url, "rb");
        if (image == NULL) {
            perror("No se ha podido abrir la isos: ");
            exit (EXIT_FAILURE);
        }

        while(!feof(image)){
            fread(sent_msg,sizeof(char),BUFF_SIZE,image);
            if(send(fdc,sent_msg,BUFF_SIZE,0) == -1){
                perror("Error al enviar el arvhivo:");
            }
        }

        bzero(argthree, BUFF_SIZE);
        bzero(filetransfer, BUFF_SIZE);
        bzero(sent_msg, BUFF_SIZE);
        transfer = false;
    }



    char txt[BUFF_SIZE] = "";
    //char file[BUFF_SIZE] = "generadorkey2";
    //calcmd5(file, md5);
    lsfile(txt);
    printf ("%s\n", txt);
    return 0;
}
