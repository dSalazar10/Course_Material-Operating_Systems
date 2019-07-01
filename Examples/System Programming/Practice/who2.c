//
//  hw2.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/4/18.
//  By default, who gathers information from the file /var/run/utmpx.
//  This program opens the utmpx file, reads entries, parses and prints info on the entry.

#include <stdlib.h>
#include <stdio.h>
//#include <stdint.h>     /* intmax_t*/
#include <utmpx.h>      /* utmpx accounting database*/

void showtime(struct timeval ut_tv)
{
    char * cp;
    
    // convert from utmpx time entry's seconds to ctime time_t
    time_t time = (time_t)ut_tv.tv_sec;
    
    // parse time using ctime
    cp = ctime(&time);
    
    // display the time
    printf("%12.12s\n", cp+4);
}

void printentry(struct utmpx * entry)
{
    /* login name */
    printf("%s\t", entry->ut_user);
    
    /* id */
    //printf("%s\n", entry->ut_id);
    
    /* tty name */
    printf("%s\t", entry->ut_line);
    
    /* process id creating the entry */
    //printf("%jd\t", (intmax_t)entry->ut_pid);
    
    /* type of this entry */
    //printf("%d\t", entry->ut_type);
    
    /* time entry was created */
    showtime(entry->ut_tv);
    
    /* host name */
    //printf("%s\n", entry->ut_host);
}

int main(int argc, char **argv)
{
    if(argc > 1)
    {
        printf("Usage: who\n");
        return -1;
    }
    // entry of a user
    struct utmpx* entry;
    
    // open database
    getutxent();
    
    // read entries
    while((entry = getutxent()))
    {
        // print only user's entry information
        printentry(entry);
    }

    // close the database
    endutxent();
    
    return 0;
}


