//
//  ch8.c
//  programs
//
//  Created by DANIEL SALAZAR on 5/26/18.
//
//  Functions: fork, exec, wait, exit

#include <stdio.h>      // printf
#include <unistd.h>     // fork, exec
#include <stdlib.h>     // exit
#include <sys/wait.h>   // wait

// returns 0 in child, PID of child in parent, -1 on error
pid_t Fork(void)
{
    /*
     In general, we never know whether the child starts executing
     before the partent, or vice versa. The order depends on the
     scheduling algorithm used by the kernel.
     
     Two reasons fork will fail:
     1) too many processes are already in the system
     2) the total number of processes for this real UID exceeds the system's limit
     
     Two uses of fork:
     1) when a process wants to duplicate itself so that the parent and the child
     can each execute different sections of the code at the same time (servers).
     2) when a process wants to execute a different program (shells). In this
     situation, it is more efficient to use the vfork() function.
     
     The kernel does the following:
     1) allocates a new chunk of memory and kernel data structures
     2) copies original process into the new one
     3) adds the new process to the set of running processes
     4) returns control back to both processes
     */
    pid_t results;
    if((results = fork()) == -1)
        perror("fork");
    return results;
}

// replaces the current process image with a new process image
// returns -1 on error, no return on success
int Execv(char *path, char *argv[])
{
    int results;
    if((results = execv(path, argv)) == -1)
        perror("Error - execv");
    return results;
}

// terminate a process
void Exit(int status)
{
    /* values of status are as follows:
     0
     EXIT_SUCCESS
     EXIT_FAILURE
     
     Before termination, exit() performs the following functions in the order listed:
     1) Call the functions registered with the atexit(3) function, in the reverse order of their registration.
     2) Flush all open output streams.
     3) Close all open streams.
     4) Unlink all files created with the tmpfile(3) function.
     */
    exit(EXIT_SUCCESS);
}

// suspends execution of its calling process until stat_loc information
// is available for a terminated child process, or a signal is received
// returns PID of a stopped or terminated child process if OK, -1 on error
pid_t Wait(int *stat_loc)
{
    /*
     if stat_loc is not a NULL pointer, the termination
     status of the terminated process is stored in it.
     
     wait does two things:
     1) it pauses the calling process until the child process finishes.
     2) it retrieves the value the child process passed to exit.
     */
    pid_t results;
    if((results = wait(stat_loc)) == -1)
        perror("Error - wait");
    return results;
}

int main(int argc, char *argv[])
{
    printf("Ch 8 HW is a perfect example.\n");
    return 0;
}
