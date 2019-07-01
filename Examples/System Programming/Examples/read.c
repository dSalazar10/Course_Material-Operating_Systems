#include "stdio.h"      // for fopen, fscanf, fclose, printf
#include "stdlib.h"     // for exit

int main (int argc, char *argv[])
{
    FILE* fp = NULL;    // file pointer

    char first[20], last[20], gender; // read data here
    int age, result;

    // open file output.txt for readingg
    fp = fopen( "output.txt", "r" );
    
    if (!fp)            // verify file opened
        exit(1);        // exit if it's not
    
    // read 2 strings, a character, and an integer from the file
    result = fscanf(fp, "%s %s %c %d", first, last, &gender, &age );

    // show what we read
    printf("%d %s %s %c %d\n", result, first, last, gender, age );   
       
    fclose(fp);         // close the file
    
    return 0;
}
