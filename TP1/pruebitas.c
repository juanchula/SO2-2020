#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

    // char recv_msg[200] = "Hola capo que tal\n";
    // int space = amountspace(recv_msg);
    // char a[200];
    // char b[200];
    // char c[200];
    char hol[200] = "hola capo asdasd asd asdasfas qwfasf qwef qasdfasd";
    strcpy(hol, "ia tu sabe");
    //sscanf(recv_msg, "%s %s %*s %*s %*s %*s %s", a, b, c);
    
    //printf("%s %i , %s, %s, %s", recv_msg,  space , a, b, c);
    printf("%s", hol);
    
    return 0;
}
