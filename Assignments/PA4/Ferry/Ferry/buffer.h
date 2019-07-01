#include <stdio.h>      // printf, perror, fclose, fopen
#include <sys/shm.h>    // shget, shmat, shmdt, shctl
#include <stdlib.h>     // exit, malloc, free, atoi, srand, strtol, rand
#include <errno.h>      // errno
#include <unistd.h>     // lseek, read, write, close
#include <string.h>     // strcpy
#include <fcntl.h>      // strcpy, O_RDONLY, O_WRONLY, open
#include <limits.h>     // UINT_MAX, PATH_MAX
#include <ctype.h>      // isalnum, ispunct
#include <pthread.h>    // PTHREAD_COND_INITIALIZER, pthread_mutex_lock, pthread_cond_wait, pthread_cond_signal,
                        // pthread_mutex_unlock, pthread_t, pthread_create

void Error(char* msg, int errnum)
{
    printf("%s", msg);
    if( errno ) perror("");
    exit(errnum);
}

// Circularly linked list
struct NODE
{
    size_t key;
    char data;
    struct NODE* prev;
    struct NODE* next;
};

struct QUEUE {
    struct NODE* head;
    struct NODE* freeList;
    int numList;
    unsigned int INFINITY;
} *queue;

// Shared Memory ID
static key_t segID = 0;
// mutex ID
pthread_mutex_t parking = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER;
pthread_cond_t can_consume = PTHREAD_COND_INITIALIZER;

// Get a new PCB node
struct NODE* GetListEl()
{
    struct NODE* p;
    if( queue->freeList != NULL )
    {
        p = queue->freeList;
        queue->freeList = queue->freeList->next;
    }
    else
    {
        p = (struct NODE*)malloc(sizeof(struct NODE));
    }
    p->key = 0;
    p->data = '\0';
    p->next = NULL;
    p->prev = NULL;
    return p;
}

// Keep it in the free list instead of freeing it
void FreeListEl(struct NODE* p)
{
    p->key = 0;
    p->data = '\0';
    p->next = queue->freeList;
    p->prev = NULL;
    queue->freeList = p;
    return;
}

void CreateFerry(unsigned int bufferSize)
{
    // Create a new shared memory segment
    if( (segID = shmget(IPC_PRIVATE, sizeof(struct QUEUE), (0660 | IPC_CREAT) )) < 0)
        Error("shmget", -1);
    // Attach the segment to queue
    if( (queue = shmat(segID, NULL, 0)) == NULL )
        Error("shmat", -6);
    // Set buffer size
    queue->INFINITY = bufferSize;
    struct NODE* p = GetListEl();
    p->next = p->prev = p;
    p->key = queue->INFINITY;
    queue->head = p;
    return;
}

void DestroyFerry()
{
    // Free all the nodes in buffer
    struct NODE* p = queue->head->next, *q;
    while( p->key != queue->INFINITY  )
    {
        q = p;
        p = p->next;
        free(q);
    }
    p->next = p->prev = p;
    queue->numList = 0;
    // Detach shared memory
    shmdt(queue->head);
    // Remove shared memory
    shmctl(segID, IPC_RMID, NULL);
    return;
}

// remove & return last node in list
char RemoveCar()
{
    // Lock
    pthread_mutex_lock(&parking);
    // Wait till data is available
    while(queue->numList == 0)
        pthread_cond_wait(&can_consume, &parking);

    // Remove a node from the end
    struct NODE* p = queue->head->prev;
    char data;
    p->next->prev = p->prev;
    p->prev->next = p->next;
    p->next = p->prev = NULL;
    queue->numList--;
    data = p->data;
    FreeListEl(p);

    // Signal and unlock
    pthread_cond_signal(&can_produce);
    pthread_mutex_unlock(&parking);
    return data;
}

// insert a node at the front of the list
void AddCar( char data )
{
    // Lock
    pthread_mutex_lock(&parking);
    // Wait till data is available
    while(queue->numList == queue->INFINITY)
         pthread_cond_wait(&can_produce, &parking);

    // Insert a node at the front of the queue
    struct NODE *e = GetListEl();
    e->key = ++queue->numList;
    e->data = data;
    e->next = queue->head->next;
    e->prev = queue->head;
    queue->head->next->prev = e;
    queue->head->next = e;

    // Signal and unlock
    pthread_cond_signal(&can_consume);
    pthread_mutex_unlock(&parking);
    return;
}
