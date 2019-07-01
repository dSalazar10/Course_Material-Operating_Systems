#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "signal.h"
#include "unistd.h"
#include "parse.h"

void prompt(char*);
void execute( char* );

int main( int ac, char* av[] )
{
    char input[255];                    // buffer for supporting command
    
    signal( SIGINT, SIG_IGN );          // ignore ctrl-c
    
    while(1)
    {
        prompt(input);
        execute( input );
    }
};

void execute( char* str)
{
    int fork_result, status, i = 0;

    char* args[255]; 
 
    parse( str, args ); // splits the user command into arguments
 
    // handle cd
    if( strcmp(args[0], "cd") == 0 )
    {
        int result = chdir(args[1]);
        
        if( result == -1 )
            perror(args[1]);
        
        return;
    }
 
    fork_result = fork();       // attempt to fork
    
    if ( fork_result == -1 )    // failure
    {
        perror("Failed to fork\n");
        exit(1);
    }
    if ( fork_result == 0 )     // I'm the child 
    {
        // execvp here
        execvp( args[0], args );
        perror("failed to exec\n");
        exit(2);
    }
    else                        // I'm the parent
    {
        // wait here
        wait( &status );        // wait for child to finish
    }
}

void prompt(char* input)
{
    printf("$ ");                                 // print prompt
    fgets( input, 255, stdin );
 
    if ( strcmp( input, "exit\n" ) == 0 )         // shell command
        exit(0);
}
