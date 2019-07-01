/* echostate.c
   reports current state of echo bit in tty driver for fd 0
   shows how to read attributes from triver and test a bit

*/

#include <stdlib.h> // for exit function
#include <stdio.h>
#include <termios.h>

int main()
{
    struct termios info;
    int rv;
    
    rv = tcgetattr( 0, &info );  // read values from the driver
    
    if ( rv == -1 )     // error reading the driver
    {
        perror( "tcgetattr" );
        exit(1);
    }
    
    // use bit mask to test the echo bit
    if( info.c_lflag & ECHO )
        printf( "echo is on, since its bit is 1\n");
    else
        printf( "echo is off, since its bit is 0\n");
        
    return 0;
}
