//
//  who2.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/4/18.
//  By default, who gathers information from the file /var/run/utmpx.
//  This program opens the utmpx file, reads entries, parses and prints info on the entry.
//  Now with buffering!

#include <stdlib.h>
#include <stdio.h>
#include <string.h>      /* strlen */
//#include <stdint.h>     /* intmax_t*/
#include <utmpx.h>      /* utmpx accounting database*/
const int MAX = 25;

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

// Copy a sting from the source to the target
void load_string(char source[], char target[])
{
    int i,j;
    i = strlen(source);
    for(j = 0; j < i; j++)
        target[j] = source[j];
    target[j] = '\0';
    return;
}

// Copy each element of the utmpx struct source to target
void load_ent(struct utmpx * source, struct utmpx * target)
{
    load_string(source->ut_user, target->ut_user);
    load_string(source->ut_id, target->ut_id);
    load_string(source->ut_line, target->ut_line);
    target->ut_pid = source->ut_pid;
    target->ut_type = source->ut_type;
    target->ut_tv = source->ut_tv;
    load_string(source->ut_host, target->ut_host);
    return;
}

int main(int argc, char **argv)
{
    if(argc > 1)
    {
        printf("Usage: who\n");
        return -1;
    }
    // single entry - buffer
    struct utmpx * entry;
    // array of entries - buffer
    struct utmpx * ent_buf[MAX];
    int i;
    
    // open database
    getutxent();
    
    // Read entries into buffer array
    for(i = 0; i < MAX && (entry = getutxent()); i++)
    {
        // allocate space in buffer
        ent_buf[i] = malloc(sizeof(struct utmpx));
        // set single entry into buffer's next element
        load_ent(entry, ent_buf[i]);
    }
    
    // print the entries from the buffer
    for(i = 0; ent_buf[i]; i++)
        printentry(ent_buf[i]);
    
    
    // close the database
    endutxent();
    
    return 0;
}


