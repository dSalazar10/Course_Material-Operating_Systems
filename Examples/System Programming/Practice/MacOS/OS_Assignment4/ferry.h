#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/****** Car Resource ******/ 
#define N 750
#define M 550

struct CAR
{
    int ticketNum, start, finish;
};

void InitCar(struct CAR* car)
{
    car->ticketNum = 0;
    
    int i = rand() % 32;
    // 4 groups, ex: 10, 20, 30
    int group = ((rand() % 4) + 1) * 10;
    // 6 units, ex: 4, 6, 1
    int end = (rand() % 6) + 1;
    // ex: 14, 26, 31
    car->finish = end + group;
    // ex: 12, 23, 30
    car->start = ((rand() % (end / 2)) + 1) + group;
}

int AddCar(struct CAR* queue)
{
    // incomplete function prototype
    return -1;
}

struct PARKING
{
    struct CAR* ptr;
    struct PARKING* next;
};

struct SCHEDULE
{
    struct PARKING *mainland, *ferry, *island;
    int mcount, fcount, icount;
} schedule;

// Manage memory with a free list
static struct PARKING* freeList = NULL;

// Create the Schedule
void CreateSchedule()
{
    // Create Island data structure
    schedule.island = (struct PARKING*)malloc(sizeof(struct PARKING));
    schedule.icount = 0;
    schedule.island->next = NULL;
    schedule.island->ptr = NULL;
    // Create Ferry data structure
    schedule.ferry = (struct PARKING*)malloc(sizeof(struct PARKING));
    schedule.fcount = 0;
    schedule.ferry->next = NULL;
    schedule.ferry->ptr = NULL;
    // Create Mainland data structure
    schedule.mainland = (struct PARKING*)malloc(sizeof(struct PARKING));
    schedule.mcount = 0;
    schedule.mainland->next = NULL;
    schedule.mainland->ptr = NULL;
    // Create freelist
    freeList = (struct PARKING*)malloc(sizeof(struct PARKING));
}
// Destroy a Parking Node
void DestroyParking(struct PARKING* head)
{
    struct PARKING* p = head, *q;
    while( p != NULL )
    {
        q = p;
        p = p->next;
        free(q->ptr);
        free(q);
    }
}
// Destroy the Schedule
void DestroySchedule()
{
    DestroyParking(schedule.island);
    DestroyParking(schedule.ferry);
    DestroyParking(schedule.mainland);
    struct PARKING* p = freeList, *q;
    while( p != NULL )
    {
        q = p;
        p = p->next;
        free(q);
    }
}

// Get a new PARKING node
struct PARKING* GetListEl()
{
    struct PARKING* p;
    if( freeList != NULL )
    {
        p = freeList;
        freeList = freeList->next;
    }
    else
    {
        p = (struct PARKING*)malloc(sizeof(struct PARKING));
    }
    p->ptr = NULL;
    p->next = NULL;
    return p;
}

// Keep it in the free list instead of freeing it to reduce memory fragmentation
void FreeListEl(struct PARKING* p)
{
    p->next = freeList;
    p->ptr = NULL;
    freeList = p;
    return;
}

// Add CAR to PARKING
void AddEl(struct PARKING* head)
{
    struct PARKING* d = GetListEl(), *p, *q;
    d->ptr = (struct CAR*)malloc(sizeof(struct CAR));
    InitCar(&d->ptr);
    for( p = head, q = NULL;
        p->ptr != NULL && p->ptr->ticketNum < d->ptr->ticketNum;
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
// Remove CAR from the PARKING
struct CAR* RemoveByKey(struct PARKING* head, int ticketNum)
{
    struct PARKING* p = head, *q = NULL;
    struct CAR* d;
    while( p != NULL && p->ptr->ticketNum != ticketNum )
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
// Search for a CAR in the PARKING using ticket number
int IsInList(struct PARKING* head, int ticketNum)
{
    struct PARKING* p = head;
    while( p != NULL && p->ptr->ticketNum < ticketNum )
        p = p->next;
    return p != NULL && p->ptr->ticketNum == ticketNum;
}
// Checks if the PCB is empty
int ParkingIsEmpty(struct PARKING* head)
{
    return head == NULL;
}
void PrintCar(struct CAR* car)
{
    printf("%d\t%d\t%d\n", car->ticketNum, car->start, car->finish);
}
// Print all elements in PARKING: <ticketNum> <start> <finish>
void PrintParking(struct PARKING* head)
{
    struct PARKING* p = head;
    printf("Ticket\tStart\tFinish\n");
    while( p != NULL )
    {
        printf("%d\t%d\t%d\n", p->ptr->ticketNum, p->ptr->start, p->ptr->finish);
        p = p->next;
    }
}



/****** End Car Resource ******/

