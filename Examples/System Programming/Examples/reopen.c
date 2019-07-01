/*
    Wrote this example on Bart ride home, hope this clears things up.
    An example of redirecting stdin and then getting it back.
    dup2 to the rescue.  Normally, you're probably going to redirect
    after forking, so you won't care about setting stdin back.
    
    However, here's how it could be done.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 256

int main()
{
    char buf[BUFSIZE];
    
    // create a duplicate connection to stdin ( backup )
    int s = dup(0);


    // close stdin
    close(0);

    // attach stdin to blah
    int fd = open("blah.txt", O_RDONLY );    
    
    // read from 0
    int bytes = read( 0, buf, BUFSIZE );
    buf[bytes] = '\0';
    
    // display what was read
    printf("bytes=%i %s\n", bytes, buf );
    
    // break connection
    close(fd); 

    // duplicate the connection to fd stdin ( fd 0 )    
    dup2( s, 0 );
    
    // close the duplicate
    close( s);
    
    // we got stdin back
    printf("write something: " );
    fgets( buf, BUFSIZE, stdin );
    printf("You wrote: %s\n", buf );
        
    return 0;
}
