/* stdinredir1.c
    purpose:    show how to redirect standard input by replacing
                file descriptor 0 with a file connection
    action:     reads 3 lines from stdin, then closes fd 0, opens
                a disk file, then reads in 3 more lines from stdin
*/

#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"

int main()
{
    int fd;
    char line[100];
    
    // read and print 3 lines
    fgets( line, 100, stdin ); printf( "%s", line );
    fgets( line, 100, stdin ); printf( "%s", line );
    fgets( line, 100, stdin ); printf( "%s", line );
    
    // redirect input
    close(0);
    fd = open( "/etc/passwd", O_RDONLY );
    
    if ( fd != 0 )
    {
        fprintf( stderr, "Could not open data as fd 0\n" );
        exit(1);
    }
    
    // read and print 3 lines -- input redirected!!!
    fgets( line, 100, stdin ); printf( "%s", line );
    fgets( line, 100, stdin ); printf( "%s", line );
    fgets( line, 100, stdin ); printf( "%s", line );
    
    return 0;
}
