#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>

#define QUEUEPATH "/pruebita" 
#define MSG_SIZE 200



int main(){
        
    // struct mq_attr queue_atributes = {0};
    // /* Setting queue atributes.
    //     *
    //     * Default and max values can be viewed/modified through /proc interfaces
    //     * explained in /proc interfaces section of man 7 mq_overview
    //     */
    // queue_atributes.mq_maxmsg  = 10 ;
    // queue_atributes.mq_msgsize = MSG_SIZE ;

    /* permission macros(the ones starting with S_) can vi viewed in man 2 open */
    mqd_t qd = mq_open(QUEUEPATH, O_RDWR);
    if (qd == -1){
            perror("Creating queue");
            exit(EXIT_FAILURE);
    }
    char sent_msg[MSG_SIZE];
    char recv_msg[MSG_SIZE];
    unsigned int prio = 1;
    do{
        printf("entre 1 \n");
        if (mq_receive(qd, recv_msg, MSG_SIZE, &prio) == -1 ){
                    perror("Receiving");
                    exit(EXIT_FAILURE);
            }
        printf("%s\n", recv_msg);
        bzero(recv_msg, MSG_SIZE);
        printf("entre 2 \n");
        fgets( sent_msg, MSG_SIZE-1, stdin );

        if (  mq_send(qd, sent_msg, MSG_SIZE, (unsigned int) 1) == -1){
            perror("Sending");
            exit(EXIT_FAILURE);
        }
        
        bzero(sent_msg, MSG_SIZE);
    }while(1);
            
    //         if (mq_close(qd) == -1){
    //                 perror("Closing queue");
    //         }
    //         printf("Message received  : %s \n", msg);
    //         exit(EXIT_SUCCESS);
    // }
    // if (mq_unlink(QUEUEPATH) == -1){
    //         perror("Destroying queue");
    // }
    return 0;

}