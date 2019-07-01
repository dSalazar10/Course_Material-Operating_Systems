/* pipedemo.c
    Demonstrates:  how to create and use a pipe
    Effect:  creates a pipe, writes into writing end, then
    runs around and reads from reading end.  A little weird,
    but demonstrates the idea.
    
    new system call:  pipe
    reminder: stdin=0, stdout=1, stderr=2
*/

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"

#define BUFSIZE 128

int main()
{
    int len, i, apipe[2];                   // 2 file descriptors
    char buf[BUFSIZ];                       // for reading end
    
    // get a pipe
    if ( pipe ( apipe ) == -1 )
    {
        perror( "could not make pipe" );
        exit(1);
    }
    
    printf( "Got a pipe!  It has file descriptors: ( %d %d )\n",
        apipe[0], apipe[1] );
        
    // read from stdin, write into pipe, read from pipe, print
    while( fgets( buf, BUFSIZ, stdin ) )
    {
        len = strlen( buf );
        
        // send down pipe
        if ( write( apipe[1], buf, len ) != len )
        {
            perror( "writing to pipe" );
            break;
        }
        
        // wipe
        for ( i = 0; i < len; i++ )
            buf[i] = 'X';
            
        // read from pipe
        len = read( apipe[0], buf, BUFSIZ );
        
        if ( len == -1 )
        {
            perror( "reading from pipe" );
            break;
        }
        
        // send to stdout
        if ( write( 1, buf, len ) != len )
        {
            perror( "writing to stdout" );
            break;
        }
    }
    
    return 0;
}
