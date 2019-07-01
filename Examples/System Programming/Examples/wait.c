// wait example, explained in depth

#include <unistd.h>
#include <wait.h>   // contains macros for testing information returned by wait
#include <stdio.h>
#include <stdlib.h>

void run_child_code();
void run_parent_code();

int main()
{
    int fork_rv;
    
    printf("Before fork:  I am %d\n", getpid() );
    
    // let's fork:
    fork_rv = fork();
    
    if ( fork_rv == 0 )
        run_child_code();
    else
        run_parent_code();
    
    return 0;
}

void run_child_code()
{
    printf( "I am the child %d - going to sleep now.\n", getpid() );
    sleep(30);
    exit(17);
}

void run_parent_code()
{
    int wait_exit_value;
    int wait_rv;
 
    printf("This is parent %d, going to wait for child.\n\n\n", getpid() );
    
    // wait returns -1 if error, or pid of the terminating process.
    // -1 if there are no processes to wait for.
    // if the argument is not NULL, then wait copies into the argument pointed
    // to the exit status or signal number.
    // values can be examined using macros in wait.h    
    wait_rv = wait( &wait_exit_value );
    
    printf("Parent %d done waiting.  Child died.  It returned %d\n\n\n",
        getpid(), wait_exit_value );
       
    // for more macros:  man wait
    printf("%d has special meaning. There are many macros", wait_exit_value );
    printf("defined in wait.h to help us translate.\n\n\n");
    printf("%d in base 10: %d\n\n\n", wait_exit_value, wait_exit_value >> 8 );
    printf( "WIFEXITED: %d\n", WIFEXITED( wait_exit_value ) );  
    printf( "WEXITSTATUS: %d\n", WEXITSTATUS( wait_exit_value ) );
    printf( "WTERMSIG: %d\n", WTERMSIG( wait_exit_value ) );
    printf( "WCOREDUMP: %d\n", WCOREDUMP( wait_exit_value ) );
}
