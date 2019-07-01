/*
 questions.c
 Daniel Salazar
 */
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit, malloc, rand
#include <errno.h>      // errno
#include <sys/shm.h>    // shget, shmat, shmdt, shctl
#include <string.h>     // strcpy, strcmp
#include <pthread.h>    // PTHREAD_COND_INITIALIZER, pthread_mutex_lock, pthread_cond_wait, pthread_cond_signal,
// pthread_mutex_unlock, pthread_t, pthread_create
#define MAX_STUDENT 4

void Error(char* msg, int errnum)
{
    printf("%s", msg);
    if( errno ) perror("");
    exit(errnum);
}
// Each student will have this
struct S_ARG
{
    char* question;
    int NetID;
};

// Prototypes
void MakeThreads(pthread_t t[], int len);
void RemoveThreads(pthread_t t[], int len);
void* ProfessorLoop();
void AnswerStart();
void AnswerDone();
void* StudentLoop(void* s);
void QuestionStart(struct S_ARG* student);
void QuestionDone(struct S_ARG* student);

// Question Global Variables
const char* nextQuestion = {"Next!"};
char* questions[255] = {
    "How do I add an image layered on top of a scene?",
    "Is it still necessary to fine tune spark config parameters?",
    "How do I fix this so that it doesn't overlay over others?",
    "Could you explain why my app fails to build?",
    "How do I deplay a website using Java?",
    "How can I display dynamic input from the user?",
    "Can I have an extension on the HW? My dog ate my computer.",
    "How do you use the navigate function?",
    "What is the correct way to use strcat()?",
    "How are we supposted to insert an element in a sorted list?",
    "Where do you like to go when you eat out?",
    "Do you like to cook?",
    "Could you live without the internet?",
    "Most memorable birthday?",
    "What would your perfect day be like?",
    "When are you happiest?",
    "What’s on your bucket list?",
    "Do you have any trips coming up?",
    "What’s something you could teach me about?",
    "What is your favorite hobby?",
    "Do you have/have you ever had any pets?",
    "What’s the best comedy movie you’ve ever seen?"
};


// Shared memory for the professor
struct Q_SYNCRO
{
    // A student's question
    char* buffer;
    // The number of students in line
    int studentCount;
} *qSynchro;

// Shared Memory ID
static key_t segID = 0;
// mutex ID
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER;
pthread_cond_t can_consume = PTHREAD_COND_INITIALIZER;

int main()
{
    // Create a new shared memory segment
    if( (segID = shmget(IPC_PRIVATE, sizeof(struct Q_SYNCRO), (0660 | IPC_CREAT) )) < 0)
        Error("shmget", -1);
    // Attach the segment to buffer
    if( (qSynchro = (struct Q_SYNCRO *)shmat(segID, NULL, 0)) == NULL )
        Error("shmat", -6);
    qSynchro->buffer = malloc(sizeof(char[255]));
    strcpy(qSynchro->buffer, nextQuestion);
    qSynchro->studentCount = 0;
    // Set up the professors and students
    int studentThreadCount = (rand() % MAX_STUDENT) + 1;
    pthread_t t[studentThreadCount + 1];
    MakeThreads(t, studentThreadCount);
    RemoveThreads(t, studentThreadCount);
    // Detach shared memory
    int shm_det = shmdt(qSynchro);
    // Remove shared memory
    shmctl(segID, IPC_RMID, NULL);
    return 0;
}

void MakeThreads(pthread_t t[], int len)
{
    int i;
    for(i = 0; i < len; ++i, qSynchro->studentCount++)
    {
        struct S_ARG* s = malloc(sizeof(struct S_ARG));
        s->NetID = i;
        s->question = questions[(rand() % 22) + 1];
        if( pthread_create(&t[i], NULL, StudentLoop, s) != 0 )
            Error("pthread", -12);
    }
    if( pthread_create(&t[len], NULL, ProfessorLoop, NULL) != 0 )
        Error("pthread", -12);
    return;
}

void RemoveThreads(pthread_t t[], int len)
{
    for(int i = 0; i <= len; ++i)
        pthread_join(t[i],NULL);
}
/*
 Professor:
 */
void* ProfessorLoop()
{
    while( qSynchro->studentCount > 0 )
    {
        if( strcmp(qSynchro->buffer, nextQuestion) != 0 )
        {
            AnswerStart();
            AnswerDone();
        }
    }
    return NULL;
}

void AnswerStart()
{// Consumer
    // Lock
    pthread_mutex_lock(&mutex);
    // Wait until a question is abailable to answer
    while( strcmp(qSynchro->buffer, nextQuestion) == 0 )
        pthread_cond_wait(&can_consume, &mutex);
    return;
}

void AnswerDone()
{
    // Elapsed time is not included
    // Answer a student's question
    printf("[P] Answers: %s\n", qSynchro->buffer);
    strcpy(qSynchro->buffer, nextQuestion);
    pthread_cond_signal(&can_produce);
    pthread_mutex_unlock(&mutex);
}

/*
 Student:
 */

void* StudentLoop(void* s)
{
    struct S_ARG* student = s;
    int done = 0;
    while( !done )
    {
        if( strcmp(qSynchro->buffer, nextQuestion) == 0 )
        {
            QuestionStart(student);
            QuestionDone(student);
            done = 1;
            printf("[S%d] Student left.\n", student->NetID);
            qSynchro->studentCount--;
            break;
        }
    }
    return NULL;
}

void QuestionStart(struct S_ARG* student)
{// Producer
    printf("[S%d] Gets in line to ask a question.\n", student->NetID);
    pthread_mutex_lock(&mutex);
    while( strcmp(qSynchro->buffer, nextQuestion) != 0 )
        pthread_cond_wait(&can_produce, &mutex);
    printf("[S%d] Asks: %s\n", student->NetID, student->question);
    strcpy(qSynchro->buffer, student->question);
    
}

void QuestionDone(struct S_ARG* student)
{
    printf("[S%d] Finished asking their question.\n", student->NetID);
    pthread_cond_signal(&can_consume);
    pthread_mutex_unlock(&mutex);
}
