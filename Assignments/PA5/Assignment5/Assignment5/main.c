
#include "job.h"
#include "stats.h"

void TestRR(void);
void TestFCFS(void);
void FCFSExample(void);

void handle_sleep_fail(int sig)
{
    // do nothing
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, handle_sleep_fail);

    InitQueue();
    int count = 0, maxCount = 200, at = 0;
    while (count < maxCount)
    {
        struct JOB *j = malloc(sizeof(struct JOB));
        j->arrival = at;
        at += rand() % 5;
        j->burst = (rand() % MAX_DIST) + 2;
        count += j->burst;
        Enqueue(j);
    }
    printf("Jobs (%d):\n", queue.length);
    printf("First-Come First-Serve\n");
    StartClock();
    TestFCFS();
    StopClock();
    PrintStats();

    printf("Round Robin\n");
    StartClock();
    TestRR();
    StopClock();
    PrintStats();

    DestroyQueue();
}

void TestRR()
{
    int time = 0, quantum = (rand() % 90) + 10;

    // Traverse the Linked List
    struct JOB *e = traverse(true);
    do
    {
        // If JOB is available
        if (e != NULL && e->burst >= quantum)
        {
            time += e->burst;
            if (e->burst % quantum > 0)
                time += quantum;
        }
    } while ((e = traverse(false)));

    nanosleep((const struct timespec[]){{0, (time * 1000000L)}}, NULL);

    return;
}

void TestFCFS()
{
    // Traverse the Linked List
    struct JOB *e = traverse(true);
    do
    {
        // If JOB is available
        if (e != NULL)
        {
            nanosleep((const struct timespec[]){{0, (e->burst * 1000000L)}}, NULL);
        }
    } while ((e = traverse(false)));
    return;
}

void FCFSExample()
{
    InitQueue();

    struct JOB *a = malloc(sizeof(struct JOB));
    a->arrival = 0;
    a->burst = 24;
    Enqueue(a);

    struct JOB *b = malloc(sizeof(struct JOB));
    b->arrival = 0;
    b->burst = 3;
    Enqueue(b);

    struct JOB *c = malloc(sizeof(struct JOB));
    c->arrival = 0;
    c->burst = 3;
    Enqueue(c);

    printf("Jobs:\n");
    PrintList();

    printf("First-Come First-Serve\n");
    StartClock();
    TestFCFS();
    StopClock();
    PrintStats();

    DestroyQueue();

    exit(1);
}
