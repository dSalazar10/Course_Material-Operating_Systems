#include "stdio.h"

int main()
{
    int c, n = 0;
    
    while( ( c = getc(stdin) ) != 'Q' )
        if ( c != '\n' )
            printf( "char %-3d is %c code %d\n", n++, c, c );
        
    return 0;
}
