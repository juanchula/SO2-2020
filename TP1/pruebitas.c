#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

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

int main()
{
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


    unsigned char key[510];
    FILE *secretkey;
    char prueba[2];
    secretkey = fopen("./isos/ejemplo.iso", "rb");
    fread(key, 1, sizeof key, secretkey);
    for(size_t j = 0; j < (sizeof(key)) ; j++) {
        if(j>=446){
            printf("%02x ", key[j]);
        }
        if(j==461 || j==477 || j==493){
            printf("\n");
        }
    }
    sscanf(key, "%02x", prueba[0], prueba[1]);
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
}
