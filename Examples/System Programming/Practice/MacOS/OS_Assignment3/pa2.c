/*
copyfile.c
Daniel Salazar
Assignment 2

This program uses a mulithreaded approach to the Producer/Consumer problem.
It has a Queue of shared memory that is accessable by both threads.
It uses mutex locks to block other threads if the shared memory is in use.
*/

#include "buffer.h"

// Prototypes
void CopyFile(char input[], char output[], int n);
void* Producer(void* args);
void* Consumer(void* args);
int WriteData(int r, int fd);
int OpenFile(char filename[], int option);
void CloseFile(int fp);

// This is used by the Producer to signal to the
// Consumer that the input file has been read completely
static int FinalIteration = 0;
// This is used in PThread_Create
// It serves as the arguments which are passed into 
// the Producer and Consumer functions
struct arg_struct {
    char file[PATH_MAX];
    int n;
};

int main(int argc, char* argv[])
{
    if( argc < 5)
    {
        printf("usage: %s <input> <output> <n> <m>", argv[0]);
        exit(1);
    }
    // Checks if n <= m, for some reason this program hangs otherwise
    if( atoi(argv[3]) > atoi(argv[4]))
        Error("m should be greater than n\n", -1);
    // Initialize random's seed
    srand( time(NULL) );
    // Initialize shared memory and the queue
    CreateBuffer((int)strtol(argv[4], (char **)NULL, 10));
    // Copy the file using PThreads, a mutex lock, and shared memory
    CopyFile( argv[1], argv[2], atoi(argv[3]) );
    // Clean up everything
    DestroyBuffer();
    return 0;
}

// Input: two files specified by the user, and a max value for random
// Process: Creates two threads of execution and two argument structs:
//          - t[0]/p for Producer
//          - t[1]/c for Consumer
// Output: N/A
void CopyFile(char input[], char output[], int n)
{
    
    pthread_t t[2];
    // Allocate memory for the args struct
    struct arg_struct* p = malloc(sizeof(struct arg_struct));
    struct arg_struct* c = malloc(sizeof(struct arg_struct));
    // copy filename over
    strcpy(p->file, input);
    strcpy(c->file, output);
    // copy n over
    p->n = n;
    c->n = n;
    // Clear output file
    fclose(fopen(output, "w"));
    // make two threads of execution, passing in the args struct
    if( pthread_create(&t[0], NULL, Producer, p) != 0 )
        Error("pthread", -12);
    if( pthread_create(&t[1], NULL, Consumer, c) != 0 )
        Error("pthread", -13);
    // Suspend the two threads
    pthread_join(t[0],NULL);
    pthread_join(t[1],NULL);
    // Deallocate the memory for the args struct
    free(p);
    free(c);
    return;
}

// Input: filename = filename provided by user
//        n = max value for random length of bytes
// Process: Copy a random # of bytes from the input file to the Queue
// Output: N/A
void* Producer(void* args)
{
    // We need to cast args back to arg struct
    struct arg_struct *a = args;
    // Set the formal arguments here
    int n = a->n;
    char filename[PATH_MAX];
    strcpy(filename, a->file);

    char* src;
    ssize_t bufsize;
    int fin = 0, index = 0, r = 0;
    
    // Open the input file and set index to 0
    fin = OpenFile(filename, O_RDONLY);
    // Read the whole file into a buffer
    // Reposition offset to end
    if( ( bufsize = lseek(fin, 0L, SEEK_END)) < 0 )
        Error("lseek", -3);
    // Allocate a buffer which can contain all the data of fin
    src = (char *)malloc((bufsize + 1) * sizeof(char));
    // Reposition offset to beginning
    if( lseek(fin, 0L, SEEK_SET) < 0 )
        Error("lseek", -4);
    // Read the whole file into memory
    if( (bufsize = read(fin, src, bufsize)) < 0 )
        Error("read", -5);
    src[bufsize] = '\0';
    // No need for fin since its in a buffer, so close it
    CloseFile(fin);
    // Copy random # of bytes from buffer into Queue
    while( index < bufsize )
    {
        r = (rand() % (n + 1)) + 1;
        if( (queue->numList + r) < queue->INFINITY )
        {
            // Push data into Queue, byte by byte
            for( int n = index + r; index <= n; ++index )
            {
                if( isalnum(src[index]) || ispunct(src[index])  || src[index] == ' ' || src[index] == '\n' )
                    WriteFront(src[index]);
            }
            // Notify Consumer of FinalIteration
            if( index >= bufsize )
                FinalIteration = 1;
        }
    }
    free(src);
    return NULL;
}

// Input: filename = filename provided by user
//        n = max value for random length of bytes
// Process: Copy a random # of bytes from the Queue to the input file
// Output: N/A
void* Consumer(void* args)
{
    // We need to cast args back to arg struct
    struct arg_struct *a = args;
    // Set the formal arguments here
    int n = a->n;
    char filename[PATH_MAX];
    strcpy(filename, a->file);
    int len, r;
    // Open the output file fout
    int fout = OpenFile(filename, O_WRONLY); 
    // Loop until Producer has finished reading the input file
    while( !FinalIteration )
    {
        r = (rand() % (n + 1)) + 1;
        if( queue->numList >= r )
            WriteData( r, fout );
    }
    if( FinalIteration && queue->numList > 0)
        WriteData( queue->numList, fout );
    CloseFile(fout);
    return NULL;
}

// Helper function for Consumer
// Writes r number of bytes to fd
int WriteData(int r, int fd)
{
    char d[r];      
    for(int j = 0; j < r; ++j)
        d[j] = ReadLast();
    lseek(fd, 0L, SEEK_END);
    return write(fd, d, r);
}

// Input: a file name and some options
// Process: Opens a file with some options
//          - open("input.txt", O_RDONLY)
//          - open("out.txt", O_WRONLY)
// Output: a file descriptor
int OpenFile(char filename[], int option)
{
    int fp;
    if( (fp = open(filename, option)) < 0 )
        Error("open", -2);
    return fp;
}

// Input: a file descriptor that was created in OpenFile
// Process: Closes a file
// Output: N/A
void CloseFile(int fp)
{
    if( close(fp) != 0 )
        Error("close", -3);
    return;
}
