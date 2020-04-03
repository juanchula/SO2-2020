#include <stdio.h>
#include <string.h>

int main()
{
    char  a[20];
    do{

        fgets( a, 20, "" );
        printf("%s", a);
        bzero(a, 20); 
    }while(1);
    
    return 0;
}
