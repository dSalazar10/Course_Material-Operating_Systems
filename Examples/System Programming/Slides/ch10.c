//
//  ch10.c
//  programs
//
//  Created by DANIEL SALAZAR on 5/27/18.
//
//  Functions: dup, dup2, pipe

#include <stdio.h>      // printf
#include <unistd.h>     // dup, dup2, pipe

// duplicate a file descriptor
// returns the new file descriptor, -1 on error
int Dup(int fd)
{
    /*
     The new fd is guaranteed to be the lowest-numbered available fd.
     
     The following:
     dup(fd);
     
     Is similar to:
     fcntl(fd, F_DUPFD, 0);
     */
    int results;
    if((results = dup(fd)) == -1)
        perror("Error - dup");
    return results;
}

// duplicate a file descriptor
// returns the new file descriptor, -1 on error
int Dup2(int fd1, int fd2)
{
    /*
     If fd2 is already open, it is first closed.
     If fd1 == fd2, this returns fd2 without closing it.
     Otherwise, the FD_CLOEXE fd flag is cleared for fd2.
     
     The following:
     dup2(fd1, fd2);
     
     Is similar to:
     close(fd2)
     fcntl(fd1, F_DUPFD, fd2);
     
     dup2 does not exactly use close(), as it is an atomic operation
     instead of using two operations.
     */
    int results;
    if((results = dup2(fd1, fd2)) == -1)
        perror("Error - dup");
    return results;
}

// fd[0] is open for reading, fd[1] is open for writing
// the output of fd[1] is the input for fd[0].
// returns 0 if OK, -1 on error
int Pipe(int fd[2])
{
    int results;
    if((results = pipe(fd)) == -1)
        perror("Error - pipe");
    return results;
}

/*    Example    */
void PipeDemo()
{
    int n, fd[2];
    pid_t pid;
    char line[4096];
    
    // create a pipe for parent and child process
    if(pipe(fd) == -1)
        perror("Error - pipe");
    
    // fork
    if((pid = fork()) == -1)
    {
        perror("Error - fork");
    }
    // parent
    else if(pid != 0)
    {
        close(fd[0]);
        write(fd[1], "Hello World!\n", 12);
    }
    // child
    else
    {
        close(fd[1]);
        n = read(fd[0], line, 4096);
        write(STDOUT_FILENO, line, n);
    }
    
    return;
}

int main(int argc, char *argv[])
{
    PipeDemo();
    return 0;
}
