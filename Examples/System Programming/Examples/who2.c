/*
    read /etc/utmp and list info therein
    suppresses empty records
    formats time nicely
*/

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SHOWHOST            // include remote machine on output

void show_info( struct utmp* );
void showtime( long );

int main()
{
    struct utmp current_record;         // read info into here
    int utmpfd;                         // read from this descriptor
    int reclen = sizeof( current_record );
    
    utmpfd = open( UTMP_FILE, O_RDONLY );
    if ( utmpfd == -1 )
    {
        perror( UTMP_FILE );                    // UTMP_FILE is in utmp.h
        exit( 1 );
    }
    
    while( read( utmpfd, &current_record, reclen ) == reclen )
        show_info( &current_record );
        
    close( utmpfd );
    return 0;                                   // went ok
        
}

void show_info( struct utmp* utbufp )
/*
    displays the contents of the utmp struct in human readable form
    displays nothing if record has no user name
*/
{
    if ( utbufp->ut_type != USER_PROCESS )      // users only
        return;
    printf("%-8.8s ", utbufp->ut_name );        // the username
    printf("%-8.8s ", utbufp->ut_line );        // the tty
    showtime( utbufp->ut_time );                // display time
#ifdef SHOWHOST
    printf("(%s)\n", utbufp->ut_host );         // the host
#endif    
}

void showtime( long timeval )
/*
    displays time in a format fit for human consumption
    uses ctime to build a string then picks parts out of it
    Note:  %12.12s prints a string 12 hars wide and LIMITS 
    it to 12 chars.
*/
{
    char* cp;                   // hold address of time
    cp = ctime( &timeval );     // convert time to string
                                // string looks like 
                                // Mon Feb  4 00:46:40 EST 1991
                                // 0123456789012345.
    printf("%12.12s ", cp+4 );  // pick 12 chars from pos 4
}


