/*  write0.c
    purpose:    send messages to another terminal
    method:     open the other terminal for output then copy from stdin
                to that terminal
    usage:      write0 ttyname
*/

#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "string.h"

#define SIZE 128

int main( int ac, char* av[] )
{
    int fd;
    char buf[SIZE];
    
    // check args
    if ( ac != 2 )
    {
        fprintf( stderr, "usage: write 0 ttyname\n" );
        exit(1);
    }
    
    // open devices
    fd = open( av[1], O_WRONLY );
    
    if ( fd == -1 )
    {
        perror( av[1] );
        exit(1);
    }
    
    // loop until EOF on input
    while ( fgets( buf, SIZE, stdin ) != NULL )
        if ( write( fd, buf, strlen( buf ) ) == -1 )
            break;
        
    close( fd );
    
    return 0;
}
