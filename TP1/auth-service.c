#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// char * convert(char *txt){
//     //char final[1024] = "<";
//     char *final = malloc()
//     strcat(final, txt);
//     strcat(final, ">");
//     char *send = strdup(final);
//     free(final);
//     return strdup(final);
// }

int searchword(char *word){
    FILE *database = fopen("/home/juanfernandez/Facu/SO2-2020/TP1/base-datos.txt", "r");
    char txt[200];
    int line = 0;

    while(fgets(txt, 200, database)){
        if(strstr(txt, word) != NULL){
            return line;
        }
        line++;
    }
    return -1;

}

bool logueo(char *user, char * password){
    char u[200] = "<";
    char userpassword[200] = "";
    int line;
    strcat(u, user);
    strcat(u, ">");
    strcat(userpassword, u);
    strcat(userpassword, "<<");
    strcat(userpassword, password);
    strcat(userpassword, ">>");
    line = searchword(user); //si no la encuentra devuelve -1
    if(line>-1){
        if(searchword(user)>-1){
            if()
        }else{
            incorrectpass(line);
            return false;
        }
    } else
        return false;


// sscanf(line, "%*s %s %*s %*s %*s %s", r, w);
}


int main()
{
    char * hola = "Hola";
    hola = convert(hola);
    printf("%s", hola);
    return 0;
}
