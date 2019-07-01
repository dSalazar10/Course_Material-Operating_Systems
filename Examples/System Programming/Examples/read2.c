#include "stdio.h"                  // for fopen, fclose, fgetc, printf
#include "stdlib.h"                 // for exit

int main (int argc, char *argv[])
{
    FILE* fp = NULL;                // file pointer
    char c;                         // read character to here
    int i;                          // for the loop
    
    fp = fopen("output.txt", "r");  // open the file for reading
    
    if (!fp)                        // verify file opened
        exit(1);                    // terminate if it didn't
    
    for ( i = 0; i < 10; i++ )      
    {
        c = getc(fp);               // read and display
        printf("%c", c);            //    10 characters
    }    
    
    fclose(fp);                     // close the file
    
    return 0;
}
