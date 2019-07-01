//
//  ch5.c
//  programs
//
//  Created by DANIEL SALAZAR on 5/26/18.
//
//  Functions: fcntl, ioctl, tcsetattr, tcgetattr

#include <stdio.h>      // printf
#include <unistd.h>     // STDIN_FILENO
#include <fcntl.h>      // fcntl
#include <sys/ioctl.h>  // ioctl
#include <termios.h>    // tcsetattr, tcgetattr

// changes the properties of an open file
// returns -1 on error
int Fcntl(int fd, int cmd)
{
    /*
     This is used for 5 different purposes:
     Duplicate an existing descriptor       F_DUPFD, F_DUPFD_CLOEXEC
     Get/set fd flags                       F_GETFD, F_SETFD
     Get/set status flags                   F_GETFL,F_SETFL
     Get/set asynchronous I/O ownership     F_GETOWN, F_SETOWN
     Get/set record locks                   F_GETLK, F_SETLK, F_SETLKW
     */
    int results;
    if((results = fcntl(fd, cmd)) == -1)
        perror("Error - fcntl");
    return results;
}

// catchall for I/O operations
// returns -1 on error, something else if OK
int Ioctl(int fd, unsigned long request)
{
    /* request types:
     terminal I/O control
     fd I/O control
     socket I/O control
     */
    int results;
    if((results = ioctl(fd, request)) == -1)
        perror("Error - ioctl");
    return results;
}

// gets the current terminal attributes
// returns 0 if OK, -1 on error
int TcGetAttr(int fd, struct termios *tp)
{
    int results;
    if((results = tcgetattr(fd, tp)) == -1)
        perror("Error - tcgetattr");
    return results;
}

// sets the current terminal attributes
// returns 0 if OK, -1 on error
int TcSetAttr(int fd, int opt, struct termios *tp)
{
    /*
     We can specify when we want the new terminal
     attributes to take effect with the following:
     TCSANOW    The change occurs immediately
     TCSADRAIN  The change occurs after all output written
                to fildes has been transmitted to the terminal.
                Changing output parameters
     TCSAFLUSH  The change occurs after all output written
                to fildes has been transmitted to the terminal.
                Discard unread input data
     */
    int results;
    if((results = tcsetattr(fd, opt, tp)) == -1)
        perror("Error - tcsetattr");
    return results;
}

/*    Example    */

void TermiosExample()
{
    struct termios term;
    
    // get current attributes
    if(tcgetattr(STDIN_FILENO, &term) == -1)
        perror("tcgetattr");
    
    // check bit size
    switch(term.c_cflag & CSIZE)
    {
        case CS5:
            printf("5 bits/byte\n");
            break;
        case CS6:
            printf("6 bits/byte\n");
            break;
        case CS7:
            printf("7 bits/byte\n");
            break;
        case CS8:
            printf("8 bits/byte\n");
            break;
        default:
            printf("Unknown bits/byte\n");
    }
    
    // zero out the bits
    term.c_cflag &= ~CSIZE;
    // set 8 bits/byte
    term.c_cflag |= CS8;
    // set the attribute immediately
    if(tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
        perror("tcsetattr");
    return;
}


int main(int argc, char *argv[])
{
    TermiosExample();
    return 0;
}
