#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define BUFF_SIZE 200
#define BASE_DATO "./base-datos"

void calcmd5(char *iso, char *md5){
    bzero(md5, BUFF_SIZE);

    //OTROS LINKS: https://stackoverflow.com/questions/1220046/how-to-get-the-md5-hash-of-a-file-in-c 
    //https://stackoverflow.com/questions/3395690/md5sum-of-file-in-linux-c
    //https://stackoverflow.com/questions/7627723/how-to-create-a-md5-hash-of-a-string-in-c
    //SACADO DE: https://stackoverflow.com/questions/10324611/how-to-calculate-the-md5-hash-of-a-large-file-in-c
    // unsigned char c[MD5_DIGEST_LENGTH];
    // char *filename="file.c";
    // int i;
    // FILE *inFile = fopen (filename, "rb");
    // MD5_CTX mdContext;
    // int bytes;
    // unsigned char data[1024];

    // if (inFile == NULL) {
    //     printf ("%s can't be opened.\n", filename);
    //     return 0;
    // }

    // MD5_Init (&mdContext);
    // while ((bytes = fread (data, 1, 1024, inFile)) != 0)
    //     MD5_Update (&mdContext, data, bytes);
    // MD5_Final (c,&mdContext);
    // for(i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i])

}

void lsfile(char *txt){
    bzero(txt, BUFF_SIZE);
    char iso[BUFF_SIZE] = "";
    char md5[BUFF_SIZE] = "";
    DIR *d = opendir("./isos");
    struct dirent *dentry;
    
    if(d != NULL){
        while((dentry=readdir(d)) != NULL){
            strcpy(iso, dentry->d_name);
            if(strstr(iso, ".iso") !=NULL){
                calcmd5(iso, md5);
            }
        }
        closedir(d);
    }else{
        perror("No se ha podido abrir el directorio: ");
        return (EXIT_FAILURE);
    }
}

int main()
{
    /* code */
    return 0;
}
