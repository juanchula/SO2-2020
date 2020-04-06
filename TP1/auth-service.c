#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFF_SIZE 200
#define BASE_DATO "/home/juanfernandez/Facu/SO2-2020/TP1/base-datos.txt"
#define BASE_DATO2 "/home/juanfernandez/Facu/SO2-2020/TP1/base-datos2.txt"

// char * convert(char *txt){
//     //char final[1024] = "<";
//     char *final = malloc()
//     strcat(final, txt);
//     strcat(final, ">");
//     char *send = strdup(final);
//     free(final);
//     return strdup(final);
// }
void changeline(int line, char *txt){
    int i = 0;
    char temp[BUFF_SIZE];
    FILE *original = fopen(BASE_DATO, "r");
    FILE *new =  fopen(BASE_DATO2, "w");
    if(original == NULL || new == NULL){
        perror("No se a podido abrir el archivo: ");
        exit(EXIT_FAILURE);
    }

    while (fgets(temp, BUFF_SIZE, original)){
        if(i==line){
            fputs(txt, new);
        }else{
            fputs(temp, new);
        }
        i++;
    }
    fclose(original);
    fclose(new);

    remove(BASE_DATO);
    rename(BASE_DATO2, BASE_DATO);
    
}

int trylogin(char *user, char * password){
    char u[BUFF_SIZE] = "<";
    char userpassword[BUFF_SIZE] = "";
    strcat(u, user);
    strcat(u, "<");
    strcat(userpassword, u);
    strcat(userpassword, ">");
    strcat(userpassword, password);
    strcat(userpassword, ">");

    char txt[BUFF_SIZE];
    int line = 0;
    char edit[BUFF_SIZE] = "";

    FILE *database = fopen(BASE_DATO, "r");
    if(database == NULL){
        perror("No se a podido abrir el archivo: ");
        exit(EXIT_FAILURE);
    }
    printf("%s %s %s.", user, password, userpassword);
    while(fgets(txt, BUFF_SIZE, database)){
        if(strstr(txt, u) != NULL){
            if(strstr(txt, userpassword) != NULL && strstr(txt, "$$$") == NULL){
                //eliminar los $
                fclose(database);
                strcat(edit, userpassword);
                strcat(edit, "\n");
                changeline(line, edit);
                return line;
            }else{
                if(strstr(txt, "$$$") == NULL){
                    strtok(txt, "\n");
                    strcat(edit, txt);
                    strcat(edit, "$");
                    strcat(edit, "\n");
                    fclose(database);
                    changeline(line, edit);
                    return -1;
                    //agregar $
                }
                break;
            }
        }
        line++;
    }
    fclose(database);
    return -1;
}

void changepassword( int line, char *user, char *newpassword){
    char userpassword[BUFF_SIZE] = "<";
    strcat(userpassword, user);
    strcat(userpassword, "<>");
    strcat(userpassword, newpassword);
    strcat(userpassword, ">\n");
    changeline(line, userpassword);
}


int main()
{
    char user[BUFF_SIZE];
    char password[BUFF_SIZE];
    int line;

    changepassword(0, "elva", "gin");
    do{
         scanf("%s",user);
         scanf("%s",password);
         //fgets( usuario, 199, stdin );
         //fgets( password, 199, stdin );
         printf("usuario ingresado:%s contraseña:%s\n", user, password);
         line = trylogin(user, password);
         printf("%i\n", line);
    }while(1);
    // char * lastuser;
    // char * lastpassword;
    // int line;
    // char * hola = "Hola";
    // hola = convert(hola);
    // printf("%s", hola);
    return 0;
}
