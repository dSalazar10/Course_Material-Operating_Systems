#include "stdlib.h"     // for malloc, free
#include "stdio.h"      // for printf

int main (int argc, char *argv[])
{
    
    int* array = NULL;  // pointer to our new array
    
    // dynamically allocate the array of 5 elements
    // 5 elements * 4 bytes for an int = 20 bytes to allocate
    array = (int*)malloc( 20 );
    
    array[0] = 5;
    array[1] = 10;
    array[2] = array[0] + array[1];
    
    printf("%d\n", array[2] );
    
    free(array);    // free the memory
    
    return 0;
}
