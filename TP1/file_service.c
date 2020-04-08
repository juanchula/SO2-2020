#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <openssl/md5.h>

#define BUFF_SIZE 200
#define DIRECTORY "./isos/"

void calcmd5(char *iso, char *md5){
    bzero(md5, BUFF_SIZE);
    char url[BUFF_SIZE] = DIRECTORY;
    strcat(url, iso);
    //printf("%s", url);
    //OTROS LINKS: https://stackoverflow.com/questions/1220046/how-to-get-the-md5-hash-of-a-file-in-c 
    //https://stackoverflow.com/questions/3395690/md5sum-of-file-in-linux-c
    //https://stackoverflow.com/questions/7627723/how-to-create-a-md5-hash-of-a-string-in-c
    //SACADO DE: https://stackoverflow.com/questions/10324611/how-to-calculate-the-md5-hash-of-a-large-file-in-c
    unsigned char c[MD5_DIGEST_LENGTH];
    FILE *inFile = fopen (url, "rb");
    MD5_CTX mdContext;
    size_t bytes;
    unsigned char data[BUFF_SIZE];

    if (inFile == NULL) {
        perror("No se ha podido abrir la isos: ");
        exit (EXIT_FAILURE);
    }

    MD5_Init (&mdContext);
    while ((bytes = fread(data, 1, BUFF_SIZE, inFile))){
        MD5_Update (&mdContext, data, bytes);
    }

    MD5_Final (c,&mdContext);
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        snprintf(md5, MD5_DIGEST_LENGTH*2, "%02x", c[i]);
        // strcat(md5, c[i]);
        printf("%02x", c[i]);
    }
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
        exit (EXIT_FAILURE);
    }
}

int main()
{
    char md5[BUFF_SIZE] = "";
    char file[BUFF_SIZE] = "generadorkey2";
    calcmd5(file, md5);
    printf ("%s\n", md5);
    return 0;
}
