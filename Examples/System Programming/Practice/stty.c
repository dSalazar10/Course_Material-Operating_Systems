#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main( int ac, char* av[] )
{
    signal( SIGINT, SIG_IGN ); // install the handler - SIG_IGN = ignore
    
    printf("You can't stop me!\n");
    while(1)  // laugh
    {
        printf("haha\n");
        sleep(1);
    }
    return 0;
}
