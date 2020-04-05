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
void changeline(int line, char *txt){
    int i = 0;
    char temp[200];
    FILE *original = fopen("/home/juanfernandez/Facu/SO2-2020/TP1/base-datos.txt", "r");
    FILE *new =  fopen("/home/juanfernandez/Facu/SO2-2020/TP1/base-datos2.txt", "w");
    if(original == NULL || new == NULL){
        perror("No se a podido abrir el archivo: ");
        exit(EXIT_FAILURE);
    }

    while (fgets(temp, 200, original)){
        if(i==line){
            fputs(txt, new);
        }else{
            fputs(temp, new);
        }
        i++;
    }
    fclose(original);
    fclose(new);

    remove("/home/juanfernandez/Facu/SO2-2020/TP1/base-datos.txt");
    rename("/home/juanfernandez/Facu/SO2-2020/TP1/base-datos2.txt", "/home/juanfernandez/Facu/SO2-2020/TP1/base-datos.txt");
    
}

int trylogin(char *user, char * password){
    char u[200] = "<";
    char userpassword[200] = "";
    strcat(u, user);
    strcat(u, "<");
    strcat(userpassword, u);
    strcat(userpassword, ">");
    strcat(userpassword, password);
    strcat(userpassword, ">");

    char txt[200];
    int line = 0;
    char edit[200] = "";

    FILE *database = fopen("/home/juanfernandez/Facu/SO2-2020/TP1/base-datos.txt", "r");
    if(database == NULL){
        perror("No se a podido abrir el archivo: ");
        exit(EXIT_FAILURE);
    }
    printf("%s %s %s.", user, password, userpassword);
    while(fgets(txt, 200, database)){
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
    char userpassword[200] = "<";
    strcat(userpassword, user);
    strcat(userpassword, "<>");
    strcat(userpassword, newpassword);
    strcat(userpassword, ">\n");
    changeline(line, userpassword);
}


int main()
{
    char user[200];
    char password[200];
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
