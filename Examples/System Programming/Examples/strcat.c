/*
    demonstrates dynamic memory allocation and 
    command-line processing.
*/

#include "stdio.h"      // printf, scanf
#include "stdlib.h"     // exit, malloc, free
#include "string.h"     // strlen, strcpy

int main (int argc, char *argv[])
{
    // check for correct syntax
    if ( argc != 3 )
    {
        printf("Usage:  strcat str1 str2\n");
        exit(1);
    }
    
    char* s = NULL;     // the new string
    int length = 0;     // the length of the new string
    
    // determine length of new string
    length = strlen( argv[1] ) + strlen( argv[2] ) + 2;
    
    // dynamically allocate memory for that string
    s = (char*)malloc(length);
    
    // construct the new string
    strcpy( s, argv[1] );
    strcpy( s + strlen(argv[1] ) + 1, argv[2] );
    s[strlen(argv[1])] = ' ';               // kill /0 for argv[1]

    // display the new string
    printf("%s\n", s);

    // deallocate memory
    free(s);
    
    return 0;
}
