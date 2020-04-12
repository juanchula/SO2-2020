#include <stdbool.h>
#ifndef _AUTH_H_
#define _AUTH_H_

    int amountspace(char * txt);
    void changeline(int line, char *txt);
    void datatime(int line);
    int trylogin(char *user, char * password);
    void changepassword( int line, char *user, char *newpassword);
    void lsuser(char * lsu);
    bool comparetxt(int length, char *txt, char *searchtxt);
    int amain();

#endif