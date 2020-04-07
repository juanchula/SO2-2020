#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <mqueue.h>

#define BUFF_SIZE 200
#define BASE_DATO "/home/juanfernandez/Facu/SO2-2020/TP1/base-datos.txt"
#define BASE_DATO2 "/home/juanfernandez/Facu/SO2-2020/TP1/base-datos2.txt"

#define QUEUEPATH "/authservice" 

// char * convert(char *txt){
//     //char final[1024] = "<";
//     char *final = malloc()
//     strcat(final, txt);
//     strcat(final, ">");
//     char *send = strdup(final);
//     free(final);
//     return strdup(final);
// }
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

void datatime(int line){
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char day_hour[128];
    strftime(day_hour,128,"%d/%m/%y %H:%M:%S",tlocal);
    strcat(day_hour, "\n");
    changeline(line, day_hour);
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
    while(fgets(txt, BUFF_SIZE, database)){
        if(strstr(txt, u) != NULL){
            if(strstr(txt, userpassword) != NULL && strstr(txt, "$$$") == NULL){
                //eliminar los $
                fclose(database);
                strcat(edit, userpassword);
                strcat(edit, "\n");
                changeline(line, edit);
                datatime(line+1);
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

void lsuser(char * lsu){
    bzero(lsu, BUFF_SIZE);
    char txt[BUFF_SIZE];
    int line = 0;
    bool habilitado;
    FILE *database = fopen(BASE_DATO, "r");
    if(database == NULL){
        perror("No se a podido abrir el archivo: ");
        exit(EXIT_FAILURE);
    }

    while(fgets(txt, BUFF_SIZE, database)){
        //strtok(txt, "\n");
        if(line % 2 == 0){
            if(strstr(txt, "$$$") != NULL){
                habilitado = false;
            }else{
                habilitado = true;
            }
            strcat(lsu, strtok(txt, "<"));
            strcat(lsu, "   ");
            if(habilitado){
                strcat(lsu, "Habilitado   ");
            }else{
                strcat(lsu, "Bloqueado   ");
            }
        }else{
            if(strstr(txt, "?")){
                strcat(lsu, "No disponible\n");
            }else{
                strcat(lsu, txt);
            }
        }
        line++;
    }
}


int main()
{
    char user[BUFF_SIZE];
    char password[BUFF_SIZE];
    int line = -1;
    int space;
    char argone[BUFF_SIZE];
    char argtwo[BUFF_SIZE];
    char argthree[BUFF_SIZE];
    char sent_msg[BUFF_SIZE];
    char recv_msg[BUFF_SIZE];

    mqd_t qd = mq_open(QUEUEPATH, O_RDWR);
    if (qd == -1){
            perror("Creating queue");
            exit(EXIT_FAILURE);
    }
    unsigned int prio = 1;

    do{

        bzero(recv_msg, BUFF_SIZE);

        if (mq_receive(qd, recv_msg, BUFF_SIZE, &prio) == -1 ){
                    perror("Receiving");
                    exit(EXIT_FAILURE);
            }

        //fgets(recv_msg, BUFF_SIZE-1, stdin);
        strtok(recv_msg, "\n");
        space = amountspace(recv_msg);
        switch (space){
        case 0: if(strstr(recv_msg, "exit") && line > -1){
                    line = -1;
                    strcpy(sent_msg, "Se ha cerrado sesion correctamente");
                }else
                    strcpy(sent_msg, "Comando incorrecto");
            break;
        
        case 1: sscanf(recv_msg, "%s %s", argone, argtwo);
                if(strstr(argone, "user") && strstr(argtwo, "ls") && line > -1){
                    lsuser(sent_msg);
                }else
                    strcpy(sent_msg, "Comando incorrecto");
                bzero(argone, BUFF_SIZE);
                bzero(argtwo, BUFF_SIZE);
            break;
    
        case 2: sscanf(recv_msg, "%s %s %s", argone, argtwo, argthree);
                if(strstr(argone, "user") && strstr(argtwo, "passwd") && line > -1){
                    strcpy(password, argthree);
                    changepassword(line, user, password);
                    strcpy(sent_msg, "Se ha cambiado la contraseña correctamente");
                }else{
                    if(strstr(argone, "login") && line == -1){
                        strcpy(user, argtwo);
                        strcpy(password, argthree);
                        line = trylogin(user, password);
                        if(line == -1){
                            strcpy(sent_msg, "Datos erroneos");
                        }else{
                            strcpy(sent_msg, "Se ha logueado correctamente");
                        }
                    }else
                        strcpy(sent_msg, "Comando incorrecto");
                }
                bzero(argone, BUFF_SIZE);
                bzero(argtwo, BUFF_SIZE);
                bzero(argthree, BUFF_SIZE);
            break;
        }
        //printf("Base de datos: %s\n", sent_msg);

        if (  mq_send(qd, sent_msg, BUFF_SIZE, (unsigned int) 1) == -1){
            perror("Sending");
            exit(EXIT_FAILURE);
        }

        bzero(sent_msg, BUFF_SIZE);
    }while(1);

    //sscanf(recv_msg, "%*s %s %*s %*s %*s %s", command, subcommand);
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
