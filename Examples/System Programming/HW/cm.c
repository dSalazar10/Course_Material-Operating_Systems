//
//  cm.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/18/18.
//
//  This program receives a file and optionally a
//  set of three characters 'r', 'w', or 'x' which
//  will represent the read, write, or execute for
//  an owner of the file. You could specify any combo
//  of the three letters and it will still build the
//  mask.

#include <stdio.h>      /* perror */
#include <fcntl.h>      /* open */
#include <unistd.h>     /* write, close */
#include <string.h>     /* strlen */
#include <sys/stat.h>   /* chmod */

#define STD_OUT 1
const int MAX = 25;

// print how to format arguments
void usage()
{
    char *u = "Usage: cm FILE [rwx]\n";
    write(STD_OUT, u, strlen(u));
}
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
    // variables
    int src;
    char pbuf[3] = "";
    struct stat st;
    mode_t mode = 0, mask = 0;
    
    // check for correct usage
    if(argc < 2 || argc > 3)
    {
        usage();
        return -1;
    }
    
    // check if file exists by opening it
    if((src = open(argv[1], O_RDONLY)) == -1)
    {
        perror("open");
        return -2;
    }
    
    // collect current file permissions
    stat(argv[1], &st);
    mode = st.st_mode;
    
    // close file
    close(src);
    
    // check if permissions require changing
    if(argc == 3)
    {
        loadOp(argv[2], pbuf);
        if(strstr(pbuf, "r"))
            mask += S_IRUSR ;
        if(strstr(pbuf, "w"))
            mask += S_IWUSR;
        if(strstr(pbuf, "x"))
            mask += S_IXUSR;
    }
    
    // change permissions
    if(chmod(argv[1], (mode & (~S_IRWXU)) | mask) == -1)
    {
        perror("chown");
        return -3;
    }
    return 0;
}
