#include <stdio.h>      // printf, perror, stdin
#include <stdlib.h>     // malloc, free, exit, waitpid, WNOHANG, wait, atoi
#include <string.h>     // strcpy, strtok strcmp
#include <errno.h>      // errno
#include <signal.h>     // signal, kill
#include <sys/wait.h>   // wait
#include <unistd.h>     // fork, execvp


#define LINESIZE 1024
#define CMDSIZE  255

// This is to be managed by the user.
// It will be used as a line for the jobs command.
struct JOB
{
    pid_t pid;
    char cmd[255];
};

// This is to be managed by the PCB
struct PCB
{
    struct JOB* ptr;
    struct PCB* next;
} *head;

// Manage memory with a free list
static struct PCB* freeList = NULL;
// Keep track of the number of elements in the list
static int numList = 0;

// Initialize head and count
void CreatePCB()
{
    numList++;
    head = NULL;
}

// Get a new PCB node
struct PCB* GetListEl()
{
    struct PCB* p;
    if( freeList != NULL )
    {
        p = freeList;
        freeList = freeList->next;
    }
    else
    {
        p = (struct PCB*)malloc(sizeof(struct PCB));
    }
    p->ptr = NULL;
    p->next = NULL;
    return p;
}

// Keep it in the free list instead of freeing it
// Reduces memory fragmentation.
void FreeListEl(struct PCB* p)
{
    p->next = freeList;
    p->ptr = NULL;
    freeList = p;
    return;
}

void DestroyPCB()
{
    struct PCB* p = head, *q;
    while( p != NULL )
    {
        q = p;
        p = p->next;
        free(q->ptr);
        free(q);
    }
    numList--;
    if( numList == 0 )
    {
        p = freeList;
        while( p != NULL )
        {
            q = p;
            p = p->next;
            free(q);
        }
    }
}

// Add JOB to the PCB
void AddEl(struct JOB* e)
{
    struct PCB* d = GetListEl(), *p, *q;
    d->ptr = e;
    for( p = head, q = NULL;
        p != NULL && p->ptr->pid < d->ptr->pid;
        q = p, p = p->next );
    if( q == NULL )
    {
        d->next = p;
        head = d;
    }
    else
    {
        d->next = p;
        q->next = d;
    }
    return;
}

// Remove JOB from the PCB
struct JOB* RemoveByKey(pid_t key)
{
    struct PCB* p = head, *q = NULL;
    struct JOB* d;
    while( p != NULL && p->ptr->pid != key )
    {
        q = p;
        p = p->next;
    }
    if( p == NULL )
        return NULL;
    else if ( q == NULL )
        head = head->next;
    else
        q->next = p->next;
    d = p->ptr;
    FreeListEl(p);
    return d;
}

// Search for a pid in the PCB
int IsInList(pid_t key)
{
    struct PCB* p = head;
    while( p != NULL && p->ptr->pid < key )
        p = p->next;
    return p != NULL && p->ptr->pid == key;
}

// Checks if the PCB is empty
int PCBIsEmpty()
{
    return head == NULL;
}

// Print all elements in the PCB: <pid> <cmd_name>
void PrintPCB()
{
    struct PCB* p = head;
    printf("PID\tCOMMAND\n");
    while( p != NULL )
    {
        printf("%d\t%s\n", p->ptr->pid, p->ptr->cmd);
        p = p->next;
    }
}

