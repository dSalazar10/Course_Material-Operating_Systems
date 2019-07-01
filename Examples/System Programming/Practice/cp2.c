//
//  cp2.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/5/18.
//

#include <stdio.h>
#include <fcntl.h>      /* open */
#include <errno.h>      /* errno */
#include <unistd.h>     /* open, read, write, close */
#include <string.h>     /* strlen */
#include <sys/stat.h>   /* stat */
#include <stdlib.h>     /* malloc */

const int MAX = 25;

// load command line argument
void loadOp(char source[], char target[])
{
    int i,j;
    i = strlen(source);
    for(j = 0; j < i; j++)
        target[j] = source[j];
    target[j] = '\0';
    return;
}

int main(int argc, char **argv)
{
    if(argc < 3 || argc > 3)
    {
        printf("Usage: cp source_file target_file\n");
        return -1;
    }
    
    // variables
    char source[MAX] = "", target[MAX] = "", *buffer;
    int src, dst, bytes;
    struct stat st;
    ssize_t bytes_read, bytes_written;
    
    // extract command line arguments
    loadOp(argv[1], source);
    loadOp(argv[2], target);
    
    // open src for reading
    if((src = open(source, O_RDONLY)) == -1)
    {
        // O_CREAT is not set and the named file does not exist
        if(errno == ENOENT)
            printf("No such src file\n");
        // Access error - permissions, non-existant file,
        else if(errno == EACCES)
            printf("You do not have permissions to open src file\n");
        return -2;
    }
    
    // open dst for writing
    if((dst = open(target, O_WRONLY)) == -1)
    {
        // O_CREAT is not set and the named file does not exist
        if(errno == ENOENT)
            printf("No such dest file\n");
        // Access error - permissions, non-existant file,
        else if(errno == EACCES)
            printf("You do not have permissions to open dest file\n");
        return -3;
    }
    
    // STAT(2): file size, in bytes
    stat(source, &st);
    bytes = st.st_size;
    
    // dynamic allocation the size of the file
    buffer = malloc(bytes * sizeof(char));
    
    // read from sourcefile to buffer
    if((bytes_read = read(src, buffer, bytes)) == -1)
    {
        int errsv = errno;
        // O_CREAT is not set and the named file does not exist
        // or
        // A read from a slow device was interrupted before any data
        // arrived by the delivery of a signal.
        if(errsv == EIO || errsv == EINTR)
            printf("Error reading file\n");
        return -4;
    }
    
    // write from buffer to copyfile
    if((bytes_written = write(dst, buffer, bytes_read)) == -1)
    {
        int errsv = errno;
        // An I/O error occurs while reading from or writing to the file system.
        // or
        // An attempt is made to write a file that exceeds the process's file
        // size limit or the maximum file size.
        if(errsv == EIO || errsv == EFBIG)
            printf("Error writing file\n");
        return -5;
    }
    
    // close sourcefile
    // ignoring possible failure to close file
    close(src);
    
    // close copyfile
    // ignoring possible failure to close file
    close(dst);
    
    return 0;
}





