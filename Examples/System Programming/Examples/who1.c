/*
    a first version of the who program
    open, read UTMP file, and show results
*/

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define SHOWHOST            // include remote machine on output

void show_info( struct utmp* );

int main()
{
    struct utmp current_record;         // read info into here
    int utmpfd;                         // read from this descriptor
    int reclen = sizeof( current_record );
    
    utmpfd = open( UTMP_FILE, O_RDONLY );
    if ( utmpfd == -1 )
    {
        perror( UTMP_FILE );    // UTMP_FILE is in utmp.h
        exit( 1 );
    }
    
    while( read( utmpfd, &current_record, reclen ) == reclen )
        show_info( &current_record );
        
    close( utmpfd );
    return 0;                           // went ok
        
}

void show_info( struct utmp* utbufp )
{
    printf("%-8.8s ", utbufp->ut_name ); // the longname
    printf("%-8.8s ", utbufp->ut_line ); // the tty
    printf("%10ld ", (long)utbufp->ut_time );  // login time
#ifdef SHOWHOST
    printf("(%s)\n", utbufp->ut_host );  // the host
#endif
    
}

