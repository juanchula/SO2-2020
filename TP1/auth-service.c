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

int trylogin(char *user, char * userpassword){
    FILE *database = fopen("/home/juanfernandez/Facu/SO2-2020/TP1/base-datos.txt", "r");
    if(database == NULL){
        perror("No se a podido abrir el archivo: ");
        exit(EXIT_FAILURE);
    }
    char txt[200];
    int line = 0;

    while(fgets(txt, 200, database)){
        if(strstr(txt, user) != NULL){
            if(strstr(txt, userpassword) != NULL && strstr(txt, "$$$") != NULL){
                //eliminar los $
                fclose(database);
                return line;
            }else{
                fclose(database);
                //agregar un $
                return -1;
            }
        }
        line++;
    }
    fclose(database);
    return -1;
}


bool login(char *user, char * password){
    char u[200] = "<";
    char userpassword[200] = "";
    int line;
    strcat(u, user);
    strcat(u, ">");
    strcat(userpassword, u);
    strcat(userpassword, "<<");
    strcat(userpassword, password);
    strcat(userpassword, ">>");
    line = trylogin(user, userpassword); //si no la encuentra devuelve -1
    return line;
// sscanf(line, "%*s %s %*s %*s %*s %s", r, w);
}


int main()
{
    char * hola = "Hola";
    hola = convert(hola);
    printf("%s", hola);
    return 0;
}
