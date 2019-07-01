/*  waitdemo2.c - shows how parent gets child status
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 5

void child_code( int );
void parent_code( int );

int main()
{
    int newpid;
    
    printf( "before:  mypid is %d\n", getpid() );
    
    if ( ( newpid = fork() )== -1 )
        perror( "fork" );
    else if ( newpid == 0 )
        child_code( DELAY );
    else
        parent_code( newpid );
    
    return 0;
}

/*
    new process takes a nap and then exits
*/
void child_code( int delay )
{
    printf( "child %d here.  will sleep for %d seconds\n", getpid(), delay );
    
    sleep( delay );
    
    printf( "child done, about to exit\n" );
    
    exit(17);
}

/*
    parent waits for child then prints a message
*/
void parent_code( int childpid )
{
    int wait_rv;                                    // return value from wait()
    int child_status;
    int high_8, low_7, bit_7;
    
    wait_rv = wait( &child_status );
    
    printf( "done waiting for %d.  Wait returned: %d\n", childpid, wait_rv );
    
    high_8 = child_status >> 8;                     // xxxx xxxx 0000 0000
    low_7 = child_status & 0x7F;                    // 0000 0000 0xxx xxxx
    bit_7 = child_status & 0x80;                    // 0000 0000 x000 0000
    
    printf( "status: exit=%d, sig=%d, core=%d\n", high_8, low_7, bit_7 );
}

/*
0000 0000 0 001 0001 = 17
--------- - --------
  |       |    |
  |       |    +-- signal ( 0 - 127 = 0000 0000 to 0111 1111 )
  |       +-- core dump
  +-- from child ( 0 - 255 = 0000 0000 to 1111 1111 )
*/
