//
//  job.h
//  Assignment5
//
//  Created by castle on 3/20/19.
//  Copyright © 2019 castle. All rights reserved.
//

#ifndef job_h
#define job_h

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_DIST 40
#define true (1==1)
#define false !true

// EXAMPLE: <0, 100>, <2, 55>, <2, 45>, <5, 10>…
struct JOB
{
    int key;
    time_t arrival;
    time_t burst;
};
struct CARRIER
{
    struct JOB *ptr;
    struct CARRIER *next;
};
struct QUEUE
{
    struct CARRIER *front, *rear, *freeList;
    int length, traverseOK, totalMs;
} queue;

struct CARRIER *getQueueEl(void)
{
    struct CARRIER *p;
    if (queue.freeList != NULL)
    {
        p = queue.freeList;
        queue.freeList = queue.freeList->next;
    }
    else
    {
        p = (struct CARRIER *)malloc(sizeof(struct CARRIER));
    }
    p->ptr = NULL;
    p->next = NULL;
    return p;
}
void freeQueueEl(struct CARRIER *p)
{
    p->next = queue.freeList;
    p->ptr = NULL;
    queue.freeList = p;
    return;
}

void InitQueue(void)
{
    queue.front = queue.rear = queue.freeList = NULL;
    queue.length = 0;
    queue.traverseOK = false;
    queue.totalMs = 0;
}

void DestroyQueue(void)
{
    struct CARRIER *p = queue.front, *q;
    while (p != NULL)
    {
        q = p;
        p = p->next;
        free(q->ptr);
        free(q);
    }
    queue.length--;
    if (queue.length == 0)
    {
        p = queue.freeList;
        while (p != NULL)
        {
            q = p;
            p = p->next;
            free(q);
        }
    }
}
void Enqueue(struct JOB *e)
{
    struct CARRIER *d = getQueueEl();
    e->key = ++queue.length;
    d->ptr = e;
    if (queue.rear == NULL)
    {
        queue.front = queue.rear = d;
    }
    else
    {
        queue.rear->next = d;
        queue.rear = d;
    }
    queue.traverseOK = false;
    queue.totalMs += e->burst;
}
struct JOB *Dequeue()
{
    if (queue.front != NULL)
    {
        struct JOB *d = queue.front->ptr;
        queue.front = queue.front->next;
        if (queue.front == NULL)
            queue.rear = NULL;
        --queue.length;
        return d;
    }
    return NULL;
}
void Insert(struct JOB *e)
{
    struct CARRIER *d = getQueueEl(), *p, *q;
    d->ptr = e;
    e->key = ++queue.length;
    for (p = queue.front, q = NULL;
         p != NULL && p->ptr->arrival < d->ptr->arrival;
         q = p, p = p->next)
        ;
    if (q == NULL)
    {
        d->next = queue.front;
        queue.front = d;
    }
    else
    {
        d->next = p;
        q->next = d;
    }
    queue.traverseOK = false;
    queue.totalMs += e->burst;
    return;
}
struct JOB *Remove(int key)
{
    struct CARRIER *p = queue.front, *q = NULL;
    struct JOB *d;
    while (p != NULL && p->ptr->key != key)
    {
        q = p;
        p = p->next;
    }
    if (p == NULL)
    {
        return NULL;
    }
    else if (q == NULL)
    {
        queue.front = queue.front->next;
    }
    else
    {
        q->next = p->next;
    }
    d = p->ptr;
    freeQueueEl(p);
    queue.traverseOK = false;
    queue.totalMs -= d->burst;
    --queue.length;
    return d;
}
void Error(char *msg, int errnum)
{
    printf("%s", msg);
    if (errno)
        perror("");
    exit(errnum);
}
void PrintList()
{
    struct CARRIER *p = queue.front;
    int i = 1;
    while (p != NULL)
    {
        printf("<%ld,%ld>\t", p->ptr->arrival, p->ptr->burst);
        p = p->next;
        if (i % 5 == 0)
            printf("\n");
        i++;
    }
    printf("\n");
    return;
}

struct JOB *traverse(int first)
{
    static struct CARRIER *p = NULL;
    if (first)
    {
        p = queue.front;
        queue.traverseOK = true;
    }
    else if (!queue.traverseOK)
    {
        p = NULL;
    }
    else if (p != NULL)
    {
        p = p->next;
    }
    if (p != NULL)
    {
        return p->ptr;
    }
    return NULL;
}

#endif /* job_h */
