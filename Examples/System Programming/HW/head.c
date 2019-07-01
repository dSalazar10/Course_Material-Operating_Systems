//
//  head.c
//
//  Created by DANIEL SALAZAR on 4/10/18.
//
//  This program is a recreation of the head program.
//  It takes in a file and writes the first 10 lines
//  to STDOUT. Optionally, an opt can be included to
//  specifiy the number of lines to be read. The program
//  will read lines, one character at a time, stopping
//  if there was a read error or it reaches EOF.

#include <stdio.h>  /* perror */
#include <ctype.h>  /* isdigit */
#include <fcntl.h>  /* open, O_RDONLY */
#include <errno.h>  /* errno, EACCES */
#include <unistd.h> /* read, write, close */
#include <string.h> /* strlen */

const int MAX = 25;
#define STD_OUT 0

// print how to format arguments
void usage()
{
    char *u = "Usage: head\n       head file.txt\n       head -n5 file.txt\n";
    write(STD_OUT, u, strlen(u));
}
// load shell arg; null terminated
void loadOp(char source[], char target[])
{
    int i,j;
    i = strlen(source);
    for(j = 0; j < i; j++)
        target[j] = source[j];
    target[j] = '\0';
    return;
}
// convert -nK (K == lineCount) to an int
int convert(char source[])
{
    int result = 0;
    int digit;
    int i = 0;
    while(source[i])
    {
        digit = (isdigit(source[i])) ? (source[i] - '0') : (0);
        result *= 10;
        result += digit;
        i++;
    }
    return result;
}
// read char by char in a single line 'till it encounters '\n'
ssize_t readLine(int src)
{
    char *c;
    ssize_t byte_count, bytes_read = read(src, c, sizeof(char));
    byte_count = bytes_read;
    while(*c != '\n' && bytes_read != 0)
    {
        write(STD_OUT, c, sizeof(char));
        bytes_read = read(src, c, sizeof(char));
        if(bytes_read == -1)
        {
            perror("read");
            return -1;
        }
        else if(bytes_read == 0)
        {
            return 0;
        }
        else
        {
            byte_count += bytes_read;
        }
    }
    write(STD_OUT, c, sizeof(char));
    return byte_count;
}

int main(int argc, char *argv[])
{
    if(argc < 2 || argc > 3)
    {
        usage();
        return -1;
    }
    // variables
    int lineCount, i = 0, src;
    char count[25] = "", source[25] = "";
    ssize_t bytes_read;
    
    // get arguments from shell
    if(argc == 2) // print 10 lines from file specified
    {
        lineCount = 10;
        loadOp(argv[1], source);
    }
    else if(argc == 3) // print count lines from file specified
    {
        loadOp(argv[1], count);
        lineCount = convert(count);
        loadOp(argv[2], source);
    }
    
    // open file
    if((src = open(source, O_RDONLY)) == -1)
    {
        // O_CREAT is not set and the named file does not exist
        if(errno == ENOENT || errno == EACCES)
            perror("open");
        return -2;
    }
    
    // echo lines to stdout
    while(i++ < lineCount && (bytes_read = readLine(src)) > 1);
    
    // close source
    close(src);
    return 0;
}
