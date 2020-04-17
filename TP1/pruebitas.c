#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#define BUFF_SIZE 1024
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

void resetchar( char * txt){
    bzero(txt, 200);
    strcat(txt, "holi");
    strcat(txt, "\n");
}

int getVal(char c)
   {
       int rtVal = 0;

       if(c >= '0' && c <= '9')
       {
           rtVal = c - '0';
       }
       else
       {
           rtVal = c - 'a' + 10;
       }

       return rtVal;
   }


int ascii_to_hex(char c)
{
        int num = (int) c;
        if(num < 58 && num > 47)
        {
                return num - 48; 
        }
        if(num < 103 && num > 96)
        {
                return num - 87;
        }
        return num;
}

void partitiontable(char *txt){
    unsigned char hex[510];
    char temp[4];
    char pruebas[1024] = ""; 
    FILE *image = fopen("./isos/ejemplo.iso", "rb");
    if(image == NULL){
            perror("No se a podido abrir el archivo: ");
            exit(EXIT_FAILURE);
        }

    fread(hex, 1, sizeof hex, image);
    for(size_t j = 0; j < (sizeof(hex)) ; j++) {
        if(j==510){
            break;
        }
        if(j>=446){
            sprintf(temp, "%02x", hex[j]);
            // printf("%02x ", hex[j]);
            strcat(txt, temp);
            strcat(txt, " ");
            printf("%s", temp);
        }
        if(j==461 || j==477 || j==493){
            printf("\n");
            strcat(txt, "\n");
        }
    }
    printf("\n");
    fclose(image);
    // printf("%s\n", pruebas);
}

void partitiontable2(char *part1, char *part2, char *part3, char *part4){
    bzero(part1, BUFF_SIZE);
    bzero(part2, BUFF_SIZE);
    bzero(part3, BUFF_SIZE);
    bzero(part4, BUFF_SIZE);
    unsigned char hex[510];
    unsigned char temp[4];
    FILE *image = fopen("./isos/ejemplo.iso", "rb");
    if(image == NULL){
            perror("No se a podido abrir el archivo: ");
            exit(EXIT_FAILURE);
        }

    fread(hex, 1, sizeof hex, image);
    for(size_t j = 0; j < (sizeof(hex)) ; j++) {
        sprintf(temp, "%02x", hex[j]);
        if(j>445 && j<462){
            strcat(part1, temp);
            strcat(part1, " ");
        }
         if(j>461 && j<478){
            strcat(part2, temp);
            strcat(part2, " ");
        }
         if(j>477 && j<494){
            strcat(part3, temp);
            strcat(part3, " ");
        }
         if(j>493 && j<510){
            strcat(part4, temp);
            strcat(part4, " ");
        }
    }
    fclose(image);
}

void partitiontable3(char *part1, char *part2, char *part3, char *part4){
    bzero(part1, BUFF_SIZE);
    bzero(part2, BUFF_SIZE);
    bzero(part3, BUFF_SIZE);
    bzero(part4, BUFF_SIZE);
    unsigned char hex[510];
    unsigned char temp[4];
    FILE *image = fopen("./isos/ejemplo.iso", "rb");
    if(image == NULL){
            perror("No se a podido abrir el archivo: ");
            exit(EXIT_FAILURE);
        }

    fread(hex, 1, sizeof hex, image);
    // for(size_t j = 0; j < (sizeof(hex)) ; j++) {
    size_t j = 509;
    while(j>445){
        sprintf(temp, "%02x", hex[j]);
        if(j>445 && j<462){
            strcat(part1, temp);
            strcat(part1, " ");
        }
         if(j>461 && j<478){
            strcat(part2, temp);
            strcat(part2, " ");
        }
         if(j>477 && j<494){
            strcat(part3, temp);
            strcat(part3, " ");
        }
         if(j>493 && j<510){
            strcat(part4, temp);
            strcat(part4, " ");
        }
        j--;
    }
    fclose(image);
}

void partitiontable4(){
    unsigned char hex[510];
    char temp[4];
    char temp4[60];
    long int size;
    int p = 4;

    FILE *image = fopen("./isos/ejemplo.iso", "rb");
    if(image == NULL){
            perror("No se a podido abrir el archivo: ");
            exit(EXIT_FAILURE);
        }
    printf("PARTICION       FIN    INICIO      TIPO        BOOTEABLE");
    printf("\n");
    fread(hex, 1, sizeof hex, image);
    // for(size_t j = 0; j < (sizeof(hex)) ; j++) {
    size_t i = 509;
    size_t j = 0;
    size_t z;
    bzero(temp4, 12);
    while(i>445){
        while(1){
            z = i + j;
            sprintf(temp, "%02x", hex[i]);
            // printf("\n%s\n", temp);
            if(z>505 && z<510){
                strcat(temp4, temp);
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
                strcat(temp4, temp);
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
                    printf("        No booteable");
                } else{
                    printf("        Booteable");
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

int main()
{
    // char a[BUFF_SIZE];
    // char b[BUFF_SIZE];
    // char c[BUFF_SIZE];
    // char d[BUFF_SIZE];
    // partitiontable3(a, b, c, d);
    // printf("%s\n%s\n%s\n%s SAPEE", a, b, c, d);
    // char algo[200]= "AA";
    // int numero = (int) strtol(algo, NULL, 16);
    // printf("\n\n %d", numero);


    partitiontable4();

    // char txt[1024];
    // partitiontable(txt);
    // printf("Holaa: %s", txt);


    // int line = 5;
    // char *txt = "ESTA NUEVA LINEA DEBE ESTAR EN 6\n";
    // int i = 0;
    // char temp[200];
    // FILE *original = fopen("/home/juanfernandez/Facu/SO2-2020/TP1/a.txt", "r");
    // FILE *new =  fopen("/home/juanfernandez/Facu/SO2-2020/TP1/b.txt", "w");
    // if(original == NULL || new == NULL){
    //     perror("No se a podido abrir el archivo: ");
    //     exit(EXIT_FAILURE);
    // }

    // while (fgets(temp, 200, original)){
    //     if(i==line){
    //         fputs(txt, new);
    //     }else{
    //         fputs(temp, new);
    //     }
    //     i++;
    // }
    // fclose(original);
    // fclose(new);

    // remove("/home/juanfernandez/Facu/SO2-2020/TP1/a.txt");
    // rename("/home/juanfernandez/Facu/SO2-2020/TP1/b.txt", "/home/juanfernandez/Facu/SO2-2020/TP1/a.txt");
    


        // time_t tiempo = time(0);
        // struct tm *tlocal = localtime(&tiempo);
        // char output[128];
        // strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
        // printf("%s\n",output);



    //char *a = "Hola capo, todo bien\n Se, todo bien.";
    // do{

    //     fgets( a, 20, "" );
    //     printf("%s", a);
    //     bzero(a, 20); 
    // }while(1);
    // FILE *database = fopen("/home/juanfernandez/Facu/SO2-2020/TP1/base-datos.txt", "r");
    // char txt[200];
    // int line = 0;

    // while(fgets(txt, 200, database)){
    //     printf("%s linea:%i",txt,line);
    //     line++;
    // }

    //char recv_msg[200] = "<elva<>gin>$$";
    //int space = amountspace(recv_msg);
    //  char a[200];
    //  char b[200];
    //  char c[200];
    //char hol[200] = "hola capo asdasd asd asdasfas qwfasf qwef qasdfasd";
    //strcpy(hol, "ia tu sabe");
    //sscanf(recv_msg, "%s %s %*s %*s %s", a, b, c);
    
    //printf("%s %i , %s, %s, %s", recv_msg,  space , a, b, c);
    //printf("%s", hol);
    
    // strcpy(a, strtok(recv_msg, "<"));
    // //strcpy(a, strtok(recv_msg, "<"));
    // printf("%s\n Mensaje que quedo:%s\n", a, recv_msg);
    // strcpy(a, strtok(NULL, ">"));
    // // strcpy(a, strtok(recv_msg, ">"));
    // printf("%s\n Mensaje que quedo:%s\n", a, recv_msg);
    // if(strstr(recv_msg, "$$$") == NULL){
    //     printf("Habilitado");
    // }

    // resetchar(recv_msg);
    // printf("%s", recv_msg);


    
    // DIR *d = opendir("./isos");
    // struct dirent *dentry;

    // if(d != NULL){
    //     while((dentry=readdir(d)) != NULL){
    //     printf("%s\n", dentry->d_name);
    //     }
    //     closedir(d);
    // }else{
    //     perror("No se ha podido abrir el directorio: ");
    //     return (EXIT_FAILURE);
    // }
    // char a[200];
    // char txt[200] = "Conectar puerto de transferencia. Datos de iso: memtest86+-5.01.iso   1839104B   0f3d162f0c2f42da1455993ac4df396b\n";
    // double b;
    // double c = 10;
    // sscanf(txt, "%*s %*s %*s %*s %*s %*s %*s %*s %s", a);
    // strtok(a, "B");
    // b = strtod(a, NULL);
    // for (int i = 0; i < c; i++)
    // {
    //     printf("%d", i);
        
    // }
    // printf ("\n%f  %f",b, c);
    // char i[1] = "";
    // FILE *original = fopen("./isoscopia/memtest86+-5.01.iso", "rb");
    // FILE *copia = fopen("/dev/sdb1", "wb");
    // if(original == NULL){
    //     perror("No se a podido abrir el archivo: ");
    //     exit(EXIT_FAILURE);
    // }
    // if(copia == NULL){
    //     perror("F: ");
    //     exit(EXIT_FAILURE);
    // }

    // while(!feof(original)){
    //     fread(i, sizeof(char), 1, original);
    //     fwrite(i, sizeof(char), 1, copia);
    // }
    // fclose(original);
    // fclose(copia);
    // printf("good\n");
    


    // FILE *fp = fopen("./isos/ejemplo.iso", "rb");
    // unsigned char temp[1000];
    // fread(temp, sizeof(temp), 1, fp);
    // for (size_t i = 0; i < 1000; i++)
    // {
    //     printf("%u\n", temp[i]);
    // }
    
    // printf("%u\n", temp[2]);


    // unsigned char key[510];
    // FILE *secretkey;
    // secretkey = fopen("./isos/ejemplo.iso", "rb");
    // fread(key, 1, sizeof key, secretkey);
    // for(size_t j = 0; j < (sizeof(key)) ; j++) {
    //     if(j>=446){
    //         printf("%02x ", key[j]);
    //     }
    //     if(j==461 || j==477 || j==493){
    //         printf("\n");
    //     }
    // }
    //printf("%u", key[498]);



        //printf("%0X%02x ", key[j]);

    // unsigned char c;
    // while (!feof(secretkey)) {
    //   c = fgetc(secretkey);
    //   printf("\n%02x", c);
    // }
    // printf("\nEnd of file\n");


    // unsigned char c1,c2;
    //     int i=0;
    //     unsigned char sum,final_hex[15/2];
    //     for(i=0;i<15/2;i++)
    //     {
    //             c1 = ascii_to_hex((char)(fgetc(secretkey)));
    //             c2 = ascii_to_hex((char)(fgetc(secretkey)));
    //             sum = c1<<4 | c2;
    //             final_hex[i] = sum;
    //             printf("%02x ",sum);
    //     }
    //     printf("\n");



    // char i[1];
    // FILE *original = fopen("./isos/debian-10.3.0-amd64-netinst.iso", "rb");
    // FILE *copia = fopen("/dev/sdb1", "r+b");
    // if(original == NULL){
    //     perror("No se ha podido abrir el archivo: ");
    //     return 0;
    // }
    // if(copia == NULL){
    //     perror("No se ha podido acceder al usb: ");
    //     return 0;
    // }

    // while(!feof(original)){
    //     fread(i, sizeof(char), 1, original);
    //     fwrite(i, sizeof(char), 1, copia);
    // }
    // fclose(original);
    // fclose(copia);
    // printf("Se ha terminado de quemar el USB\n");
    // __pid_t pidauth;
    // __pid_t pidfile;
    // printf("hola kapo 1\n");
    // pidauth = fork();
    // printf("hola kapo 2: %d\n", getpid());
    // if(pidauth>0){
    //     printf("soy el padre: %d, %d\n", getpid(), pidauth);
    //     pidfile = fork();
    // }
    // if(pidauth == 0){
    //     printf("soy el auth: %d\n", getpid());
    // }
    // if(pidfile == 0 && pidauth>0){
    //     printf("soy el file: %d, %d\n", getpid(), pidauth);
    // }
    
    // if(pidauth>0 && pidfile>0){
    //     printf("soy el padre: %d, %d\n", getpid(), pidauth);
    // }
    // sleep(10);


    
    return 0;
}
