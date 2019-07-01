/*
tish.c
Tiny Shell Usage: tish>> <command_name> <list_of_parameters>

Services:
- Running programs in foreground halts the shell prompt until program ends
- Running programs in background allows for immediate use of the shell after execution
- List all processes that currently run in the background
- Kill a process running in the background
- Run external and internal commands

Internal Commands:
- "bye": terminate all threads and then terminate tish
- "jobs": list all background programs sorted by order of creation (<pid> <command_name>)
- "kill": terminate the process corresponding to the specified pid by sending SIGTERM signal
Foreground Programs: <command_name> <list_of_parameters>
- Pause tish and execute any program on the system
Background Programs: <command_name> <list_of_parameters> &
- Init a new thread of the program

Requirements:
+ You are required to use fork() and execvp()
+ list_of_parameters.lenght() <= TwoElements
+ Type checking for execvp() is not required
+ system() is strictly prohibited
+ Error handling on input with proper output
+ Ignore whitespace as a command_name; prompt immediately
+ tish ignores SIGINT and catches SIGTERM, foreground program gets terminated
+ Ignore limit of zombie processes
+ tish input is limited to 1024 characters

System Calls and Functions List
+ fork()
+ execvp()
+ wait()
+ signal()
+ kill()
+ exit()
+ strtok()
*/

#include "pcb.h"    // Custom PID Accounting JOB Structure

// Prototypes
ssize_t GetInput(char *input);
int ParseInput(char *buffer, char* pList[]);
int Prompt(char* pList[]);
void Execute(int argv, char* argc[]);
void Execute0Args(char* argv[]);
void Execute1Args(char* argv[]);
void Execute2Args(char* argv[]);
void Execute3Args(char* argv[]);
void PerformExe(char* argv[], int background);

/*
TODO:
- Fix Signals
- Test everything
*/

void Error(char* msg, int errnum)
{
    printf("%s", msg);
    if( errno ) perror("");
    exit(errnum);
}
void SigHandler(int signo)
{
    if( signo == SIGTERM )
        printf("received SIGTERM\n");
}

int main(int argc, char* argv[])
{
    // Variables
    char* pList[CMDSIZE];
    int ntoks = 0;
    CreatePCB();

    // Ignore SIGINT
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
        Error("signal failed\n", -9);
    // Catch SIGTERM
    if (signal(SIGINT, SigHandler) == SIG_ERR)
        Error("signal failed\n", -9);

    while(1)
    {
        ntoks = Prompt(pList);
        if( ntoks > 0 ) Execute(ntoks, pList);
    }
    return 0;
}

// Input: an empty char array, to be used for input
// Process: prompts user and store results in array
// Output: the number of characters read by getline
ssize_t GetInput(char *buffer)
{
    // Variables
    size_t len;
    ssize_t bytes_read;
    char* line = (char *)malloc(CMDSIZE * sizeof(char));

    // Print tish shell prompt
    printf("tish>> ");
    // Get and store input in buffer
    fpurge(stdin);
    if( (bytes_read = getline(&line, &len, stdin)) == -1 )
        printf("Blank Line!\n");
    // Ignore whitespace for the first character in input
    if( line[0] == ' ' || line[0] == '\t' || line[0] == '\n')
        return -1;
    // Restricts input to 1024
    if( bytes_read > LINESIZE )
        Error("Error: Invalid Line Length", -1);
    // Copy inout into buffer
    strcpy(buffer, line);
    // Add the null terminator
    buffer[bytes_read - 1] = '\0';
    return bytes_read;
}

// Input: array with user input and an empty 2D array
// Process: Tokenizes the buffer, separating by spaces, 
//          updating pList with tokens
// Output: The number of tokens that were parsed from buffer
int ParseInput(char* buffer, char* pList[])
{
    int i = 0;
    // Priming Reed
    pList[i] = strtok(buffer, " ");
    // Continuing Reed
    while( (pList[++i] = strtok(NULL, " ")) != NULL );
    if( i > 4 ) 
        Error("list of parameters cannot exceed 3: \n\texample_prog param1 param2 param3\n", -4);
    return i;
}

// Input: an empty 2D array, to be used for tokens
// Process: gets a line from stdin and tokenizes it
// Output: the number of tokens parse from user input
int Prompt(char* pList[])
{
    // Variables
    char buffer[LINESIZE];
    int char_count, arg_count;
    // Prompt the user for input
    if( (char_count = GetInput(buffer)) == -1 )
        return -1;
    // Parse the input into tokens
    arg_count = ParseInput(buffer, pList);
    return arg_count;
}


// 
void Execute(int argc, char* argv[])
{
    char* args[CMDSIZE];
    for(int i = 0; i < argc; ++i)
    {
        args[i] = (char *)malloc(CMDSIZE * sizeof(char));
        strcpy(args[i], argv[i]);
    }

    // Execution for each possibility
    switch(argc)
    {
        case 1:
        Execute0Args(args);
        break;

        case 2:
        Execute1Args(args);
        break;

        case 3:
        Execute2Args(args);
        break;

        case 4:
        Execute3Args(args);
        break;

        default: Error("Illegal number of arguments", -4);
    }
}

// Input: the tokenized input from user
// Helper Function: performs the process specified by the user
// Output: N/A
void Execute0Args(char* argv[])
{
    if( strcmp(argv[0], "bye") == 0 )
    {
        while( !PCBIsEmpty() )
        {
            struct JOB* d = RemoveByKey(head->ptr->pid);
            printf("Terminating <%d>\t<%s>\n", d->pid, d->cmd);
            kill(d->pid, SIGTERM);
        }
        DestroyPCB();
        exit(0);
    }
    else if( strcmp(argv[0], "jobs") == 0 )
    {
        if( !PCBIsEmpty() )
            PrintPCB();
        else
            printf("There are zero jobs running in the background.\n");        
    }
    else
    {
        PerformExe(argv, 0);
    }
}
void Execute1Args(char* argv[])
{
    if( strcmp(argv[0], "kill") == 0 )
    {
        if( !PCBIsEmpty() )
        {
            // Get the JOB that corresponds to the pid user wants to kill
            struct JOB* d = RemoveByKey(atoi(argv[1]));
            if( d != NULL )
            {
                printf("Terminating %s\n", d->cmd);
                // Ignore error handling here because the process may already be closed;
                // this is just a simulation so we dont really care if this works or not
                kill(d->pid, SIGTERM);
            }
            else
            {
                printf("Invalid PID\n");
            }
        }
    }
    else if( strcmp(argv[1], "&") == 0 )
    { // command_name + &
        PerformExe(argv, 1);
    }
    else
    {// command_name + 1 param
        PerformExe(argv, 0);
    }
}
void Execute2Args(char* argv[])
{
    if ( (strcmp(argv[1], "&")) != 0 && (strcmp(argv[2], "&")) != 0)
    { // command_name + 2 params
        PerformExe(argv, 0);
    }
    else if( (strcmp(argv[1], "&") != 0 && strcmp(argv[2], "&")) == 0 )
    { // command_name + 1 param + &
        PerformExe(argv, 1);
    }
    else
    {
        Error("Usage: <command_name> <list_of_parameters> [&]", -5);
    }
}
void Execute3Args(char* argv[])
{
    if ( (strcmp(argv[1], "&")) != 0 && (strcmp(argv[2], "&")) != 0 && (strcmp(argv[3], "&")) == 0)
    { // command_name + 2 params
        PerformExe(argv, 0);
    }
    else if( (strcmp(argv[1], "&") != 0 && strcmp(argv[2], "&")) == 0 )
    { // command_name + 2 params + &
        PerformExe(argv, 1);
    }
    else
    {
        Error("Usage: <command_name> <list_of_parameters> [&]", -5);
    }
}


void PerformExe(char* argv[], int background)
{
    pid_t pid;
    int status;

    if( (pid = fork()) == -1)
        Error("Fork failed\n", -6);
    
    
    if( pid == 0 )
    {// Child
        if( !background )
        {
            execvp(argv[0], argv);
            Error("execvp failed\n", -7);
        }
        exit(0);
    }
    else
    {// Parent
        if( !background )
        {
            wait(&status);
            return;
        }
        // Create a new JOB node and add it to the PCB
        struct JOB* d = (struct JOB*)malloc(sizeof(struct JOB));
        d->pid = pid;
        strcpy(d->cmd, argv[0]);
        AddEl(d);
        return;
    }
}