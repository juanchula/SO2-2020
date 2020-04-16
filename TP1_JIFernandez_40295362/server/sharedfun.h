/**
 * @file sharedfun.h
 * @author Juan Ignacio Fernandez
 */

#include <stdbool.h>
#ifndef _SHAREDFUN_H_
#define _SHAREDFUN_H_

    /**
     * @brief Calcula la cantidad de espacios del contenido del arreglo de char
     * @param txt Puntero de arreglo de char a procesar
     * @return Numero de espacios
     */
    int amountspace(char * txt);

    /**
     * @brief Verifica si 2 arreglo de char contienen lo mismo
     * @param length Longitud de la frase/palabra a comparar
     * @param txt Puntero de arreglo de char a comparar
     * @param searchtxt Puntero de arreglo  de char a comparar
     * @return True si los contenidos son iguales y false si no
     */
    bool comparetxt(int length, char *txt, char *searchtxt);

#endif