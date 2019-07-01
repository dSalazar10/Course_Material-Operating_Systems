#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>

#define MSGMAX 4056
#define TRUE (1==1)
#define FALSE (!TRUE)

void Error(char* msg, int errnum)
{
    printf("%s", msg);
    if( errno ) perror("");
    exit(errnum);
}
struct mymsgbuf {
    long mtype;
    char mtext[MSGMAX];
} msg;

//  creates or establishes a connection to a message channel; returns a channel descriptor
int open( key_t keyval )
{
    return msgget(keyval, IPC_CREAT | 0777);
}
// Remove the message Queue
int close( int qid )
{
    return msgctl( qid, IPC_RMID, 0);
}
// sends message through a channel (one of the arguments is a descriptor returned by msgget).
int write( int qid, struct mymsgbuf* qbuf )
{
    int length = sizeof(struct mymsgbuf) - sizeof(long);        
    return msgsnd(qid, qbuf, length, 0);
}
// receives message (using a descriptor returned from a previous call to msgget).
int read( int qid, long type, struct mymsgbuf *qbuf )
{
    int length = sizeof(struct mymsgbuf) - sizeof(long);        
    return msgrcv(qid, qbuf, length, type,  0);
}
// Check if a message exists in the Queue
int peek( int qid, long type )
{
    int result = msgrcv( qid, NULL, 0, type,  IPC_NOWAIT);
    return (result == -1 && errno == E2BIG) ? (TRUE) : (FALSE);
} 
// Copy information from the kernel data structure associated 
// with msqid into the msqid_ds structure pointed to by qbuf.
int get_queue_ds( int qid, struct msqid_ds* qbuf )
{
    return msgctl(qid, IPC_STAT, qbuf);
}
// Read the kernel msqid_ds, change the mode
// and write it back to kernel msqid_ds
int change_queue_mode( int qid, char *mode )
{
        struct msqid_ds tmpbuf;
        get_queue_ds(qid, &tmpbuf);
        sscanf(mode, "%ho", &tmpbuf.msg_perm.mode);
        return msgctl(qid, IPC_SET, &tmpbuf);
}
void Example()
{
    int qid, msg_queue;
    key_t msgkey = ftok(".", 'm');

    // Get a message Queue
    if( (msg_queue = open(msgkey)) == -1 )
    {
        perror("open_queue");
        exit(1);
    }
    if( (write( qid, &msg )) == -1 )
    {
            perror("send_message");
            exit(1);
    }
    return;
}
