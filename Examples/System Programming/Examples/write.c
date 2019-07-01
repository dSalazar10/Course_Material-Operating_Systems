#include "stdio.h"      // for fopen, fprintf, fclose
#include "stdlib.h"     // for exit

int main (int argc, char *argv[])
{
    FILE* fp = NULL;    // file pointer
    int age = 22, num;
    char gender = 'M', name[] = "John Doe";

    // open file output.txt for writing
    fp = fopen( "output.txt", "w" );
    
    if (!fp)            // verify file opened
        exit(1);        // exit if it's not
    
    // write to the file
    num = fprintf( fp, "%s %c %d\n", name, gender, age );
    
    fclose(fp);         // close the file
    
    return 0;
}
