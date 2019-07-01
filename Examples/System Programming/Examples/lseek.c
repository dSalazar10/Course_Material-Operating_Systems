/*  lseek.c - demonstrates how to use lseek to move the file pointer
    within a file.
*/


#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

int main( int ac, char** av )
{
    char buffer[128];
  
    // open file
    int fd = open( av[1], O_RDONLY );
 
    // move 20 bytes forward within the file
    lseek( fd, 20, SEEK_SET );
    
    // read 128 bytes from the file into buffer
    read( fd, buffer, 128 );
    
    // print contents of buffer
    printf("%s\n", buffer );
    
    // move 20 bytes back in file
    lseek( fd, -20, SEEK_CUR );
    
    // read 128 bytes from the file into buffer
    read( fd, buffer, 128 );
    
    // print contents of buffer
    printf("\n\n\n\n\n\n%s\n", buffer );
    
    close( fd );

    return 0;
}
