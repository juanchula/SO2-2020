#include <stdbool.h>
#ifndef _FILESERV_H_
#define _FILESERV_H_

    void calcmd5(char *iso, char *md5);
    long int filesize(char *iso);
    void lsfile(char *txt);
    bool fileverification(char *filename, char *txt);
    void sendiso(char * argthree, int fdc, char *filetransfer);
    int fsmain();

#endif