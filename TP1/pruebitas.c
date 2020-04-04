#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
    //char *a = "Hola capo, todo bien\n Se, todo bien.";
    // do{

    //     fgets( a, 20, "" );
    //     printf("%s", a);
    //     bzero(a, 20); 
    // }while(1);
    FILE *database = fopen("/home/juanfernandez/Facu/SO2-2020/TP1/base-datos.txt", "r");
    char txt[200];
    int line = 0;

    while(fgets(txt, 200, database)){
        printf("%s linea:%i",txt,line);
        line++;
    }
    return 0;
}
