/**
 * @file auth.h
 * @author Juan Ignacio Fernandez
 */

#include <stdbool.h>
#ifndef _AUTH_H_
#define _AUTH_H_

    /**
     * @brief Intercambia la linea "line" de la base de datos por el arreglo de char "txt"
     * @param line Numero de linea a cambiar
     * @param txt Puntero al arreglo de char que se insertara en la linea del archivo
     */  
    void changeline(int line, char *txt);

    /**
     * @brief Agrega a la fecha/hora actual en la linea "line" de la base de datos
     * @param line  Numero de linea a cambiar
     */    
    void datatime(int line);

    /**
     * @brief Prueba loguearse con el usuario y contraseña que recibe. Modifica la base de datos si es necesario
     * @param user Puntero al arreglo de char que contiene el usuario
     * @param password Puntero al arreglo de char que contiene la contraseña
     * @return -1 si no se pudo conectar y el numero de linea de la base de datos donde estan los datos del usuario si se pudo loguear
     */    
    int trylogin(char *user, char * password);

    /**
     * @brief Cambia la contraseña en la base de datos del acutal usuario coenctado
     * @param line Numero de lnea de la base de datos del actual usuario conectado
     * @param user Puntero de arreglo de char que contiene el nombre de usuario
     * @param newpassword Puntero de arreglo de char que contiene la nueva contraseña
     */    
    void changepassword( int line, char *user, char *newpassword);

    /**
     * @brief Guarda en lsu lista de usuario, estado y ultima conexion
     * @param lsu Puntero de arreglo de char donde guardara la lista
     */    
    void lsuser(char * lsu);
        
    /**
     * @brief Funcion principal de el servicio de autentificacion. Procesa los comandos que recibe de el servidor principal
     * @return 0
     */    
    int amain();

#endif