#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "signal.h"
#include "unistd.h"
#include "parse.h"
#include "fcntl.h"

void prompt(char*);
void execute( char*[] );
int scan( char*[], char );
void do_execute( char*[] );

int main( int ac, char* av[] )
{
    char input[255];                    // buffer for supporting command
    char* args[255];
    
    signal( SIGINT, SIG_IGN );          // ignore ctrl-c
    
    while(1)
    {
        prompt(input);
        parse( input, args );
        execute( args );
    }
};

void execute( char* args[] )
{
    int fork_result, status, i = 0;
    if( strcmp(args[0], "cd") == 0 )
    {
        int result = chdir(args[1]);
        if( result == -1 )
            perror(args[1]);
        return;
    }
    fork_result = fork();
    if ( fork_result == -1 )
    {
        perror("Failed to fork\n");
        exit(1);
    }
    if ( fork_result == 0 )
    {
       do_execute( args );
    }
    else
    {
        wait( &status );
    }
}

void prompt(char* input)
{
    printf("$ ");                                 // print prompt
    fgets( input, 255, stdin );
 
    if ( strcmp( input, "exit\n" ) == 0 )         // shell command
        exit(0);
}

// scans the array of arguments for a particular character
// used to identify position of <, >, |
int scan( char* args[], char t )
{
    int i;
    for( i = 0; args[i] != '\0'; i++ )
        if( args[i][0] == t )
            return i;
    
    return -1;
}

void do_execute( char* args[] )
{  
   int pos;

    // handle | here ( fork your fork )
    if( ( pos = scan( args, '|' ) ) != -1 )
    {
        int apipe[2];
        int pipe_result;

        // pipe...
        pipe_result = pipe( apipe );

        // ...then fork
        int fork_result = fork();

        // grandchild -- gonna read
        if( fork_result > 0 )
        {
           // set up file descriptors
           close( apipe[1] );
           dup2( apipe[0], 0 );   
           close( apipe[0] );

           // execute the read half of the pipe
           do_execute( &args[pos+1] );
        }
        else if( fork_result == 0 ) // child -- gonna write
        {
           // setup file descriptors
           close( apipe[0] );
           dup2( apipe[1], 1 );
           close( apipe[1] );

           // execute the write half of the pipe
           args[pos] = '\0';
           do_execute( args );
        }
        else
           perror("failed to fork the fork\n");
    }
  
    // handle < here 
    else if(  ( pos = scan( args, '<' ) ) != -1 )
    {   
        // close...
        close(0);

        // ...then open
        int result = open( args[pos + 1] , O_RDONLY );

        if( result == -1 )
            perror("Error opening file.\n");
        else
            args[pos] = '\0';
    }

    // handle > here
    else if( ( pos = scan( args, '>' ) )!= -1 )
    {
         // close...
        close(1);

        // ...then open
        int result = creat( args[pos+1], 0644 );

        if( result == -1 )
            perror( "Error opening file.\n" ); 
        else
            args[pos] = '\0';
    }

    // execvp here
    execvp( args[0], args );
    perror("failed to exec\n");
    exit(2);
}
