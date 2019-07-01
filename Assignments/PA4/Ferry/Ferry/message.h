//
//  message.h
//  Ferry
//
//  Created by castle on 3/18/19.
//  Copyright © 2019 castle. All rights reserved.
//

#ifndef message_h
#define message_h

#include <sys/msg.h>
#define MSGMAX 4056

struct mymsgbuf {
    long mtype;
    unsigned int space;
} msg;
//  creates or establishes a connection to a message channel; returns a channel descriptor
int MsgOpen(key_t key)
{
    return msgget(key, IPC_CREAT | 0666);
}
// Remove the message Queue
void MsgClose(int msgID)
{
    if(msgctl( msgID, IPC_RMID, 0) != 0)
        Error("msgctl", 1);
    return;
}
// sends message through a channel (one of the arguments is a descriptor returned by msgget).
void WriteMsg(unsigned int space, int msgID)
{
    msg.space = space;
    // Ignore error handling
    msgsnd(msgID, &msg, sizeof(unsigned int), 0);
    return;
}
// receives message (using a descriptor returned from a previous call to msgget).
unsigned int ReadMsg(int msgID)
{
    // Ignore error handling
    msgrcv(msgID, &msg, sizeof(unsigned int), msg.mtype,  0);
    return msg.space;
}

#endif /* message_h */
