#include "sharedfun.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

bool comparetxt(int length, char *txt, char *searchtxt){
    if(length == (int) strlen(txt) && strstr(txt, searchtxt) != NULL){
        return true;
    }else{
        return false;
    }
}