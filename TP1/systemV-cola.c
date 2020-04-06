#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/msg.h>

struct usrmsg {
        int id;
        char info;
};

struct msgbuf {
        long mtype; /* mandatory queue message member*/
        struct usrmsg msg;
};

// struct msgbuf
//   {
//     __syscall_slong_t mtype;	/* type of received/sent message */
//     char mtext[1];		/* text of the message */
//   };
  

int main(){
    struct msgbuf parent_msg = {MSGTYPE, {10,'A'}};
    
    key_t qkey = ftok("/home/juanfernandez/Facu/SO2-2020/TP1/generadorkey", 66);
    if(qkey == -1){
        perror("Error al generar la clave para la cola : ");
        exit(EXIT_FAILURE);
    }

    qid = msgget(qkey, 0666 | IPC_CREAT)
    if(qid == -1){
        perror("Error al generar la cola: ");
        exit(EXIT_FAILURE);
    }

    msgsnd(qid, ) //enviar
    msgrcv(qid, ) //reciir
    return 0;
}
