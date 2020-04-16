/**
 * @file fileserv.h
 * @author Juan Ignacio Fernandez
 */

#include <stdbool.h>
#ifndef _FILESERV_H_
#define _FILESERV_H_

    /**
     * @brief Calcula el MD5HASH de un archivo
     * @param iso Puntero al arreglo de char que contiene el nombre del archivo
     * @param md5 Puntero al arreglo de char donde se cargara el MD5
     */
    void calcmd5(char *iso, char *md5);

    /**
     * @brief Calcula el tamaño de un archivo en bytes
     * @param iso Puntero al arreglo de char que contiene el nombre del archivo
     * @return Tamaño del archivo
     */
    long int filesize(char *iso);

    /**
     * @brief Guarda en txt la lista de archivos .iso acompañado de su peso y MD5
     * @param txt Puntero de arreglo de char donde guardara la lista.
     */
    void lsfile(char *txt);

    /**
     * @brief Verifica si existe el archivo .iso y si existe guarda la informacion de este (nombre, tamaño y MD5)
     * @param filename Puntero de arreglo de charcon el nombre del iso
     * @param txt Puntero de arreglo de char donde se guardara la informacion
     * @return True si el archivo existe y termina en .iso y false en caso contrario
     */
    bool fileverification(char *filename, char *txt);

    /**
     * @brief Realiza la transferencia del iso por socket
     * @param argthree Puntero de arreglo de char que contiene el nombre del iso
     * @param fdc File descriptor del socket
     * @param filetransfer Puntero de arreglo de char que contiene el tamaño del archivo entre otras cosas
     */
    void sendiso(char * argthree, int fdc, char *filetransfer);

    /**
     * @brief Funcion principal de el servicio de archivos. Procesa los comandos que recibe de el servidor principal
     * @return 0
     */
    int fsmain();

#endif