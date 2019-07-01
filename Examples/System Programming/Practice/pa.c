//
//  pa.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/7/18.
//


#include <stdio.h>
#include <unistd.h>     /* acct */
#include <errno.h>      /* errno */
#include <fcntl.h>      /* creat, S_IRWXU, S_IRGRP, S_IROTH */
#include <sys/acct.h>   /* struct acct */
//  Logs process accounting information to a file specified on the command line.
//  If no filename is specified, process accounting is switched off.
int enable_accounting(int turn_off, char filename[])
{
    int rc;
    if(turn_off == 1)
    {
        printf("Attempting to turn off process accounting.\n");
        if((rc = acct(NULL)))
        {
            // Function not implemented
            if(errno == ENOSYS)
                printf("It appears your kernel does not include accounting support\n");
            else
                perror("Problem turning off accounting");
            return rc;
        }
    }
    else
    {
        printf("Attempting to log to file %s.\n", filename);
        rc = creat(filename, S_IRWXU | S_IRGRP | S_IROTH);
        if(rc == -1)
        {
            perror("Problem creating log file");
            return rc;
        }
        if((rc = acct(filename)))
        {
            perror("Problem in acct() call");
            return rc;
        }
    }
    return 0;
}

// reading a process accounting record into memory.
int read_pa(char filename[])
{
    // accounting record
    struct acct a;
    // file descriptor
    int fd;
    // buffer
    int bytesread;
    // open accounting record
    if((fd = open(filename, O_RDONLY)) == -1)
    {
        perror ("Problem opening specified file");
        return -1;
    }
    // todo: create a buffer to read multiple accounting records at once
    printf("Records:\n");
    while ((bytesread = read(fd, &a, sizeof(struct acct))) > 0)
        printf("%s\n", a.ac_comm);
    return 0;
}

int main (int argc, char **argv)
{
    int rc;
    if (argc == 1)
    {
        int rc;
        rc = enable_accounting(1, argv[0]);
        if(rc == 0)
            printf("Successfully turned off accounting\n");
    }
    else if(argc == 2)
    {
        // todo: switch to getopt style
        int num, nitems, rc;
        printf("\n1)Enable Accounting\n2)Read Record\nMake a choice: ");
        
        nitems = scanf("%d", &num);
        if (nitems == EOF || nitems == 0)
        {
            printf("Failed to get input\n");
        }
        else
        {
            if(num == 1)
            {
                rc = enable_accounting(0, argv[1]);
                if(rc == 0)
                    printf("Successfully turned on accounting\n");
            }
            else
            {
                rc = read_pa(argv[1]);
            }
        }
    }
    else
    {
        printf("Too many arguments.");
        return -1;
    }
    return 0;
}





