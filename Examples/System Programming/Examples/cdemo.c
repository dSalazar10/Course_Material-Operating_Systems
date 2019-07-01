/*
    to compile this demo program:
    gcc cdemo.c -o c
    
    gcc: invokes the command-line compiler
    cdemo.c:  the name of the source code file to compile
    -o: a switch that specifies the name of the executable
    c:  the executable name
    
    to run the compiled program:
    ./c from the command line
*/

#include "stdio.h"          // gets us printf, scanf




// argc contains the number of arguments at the command line
// argv is an array of c-strings.  Each element contains one
// of the argments.
// for example, ./c hello world is 3 arguments.
// ./c goes in element 0, hello in element 1, world in element 2
// null terminator goes in element 3
int main (int argc, char* argv[])
{
    // process command line arguments
    int i;
    
    // %s is a format specifier, a placeholder for strings argv[i] 
    // other examples: %c = char, %d = int, %f = double, float
    for ( i = 0; i < argc; i++ )
        printf("%s\n", argv[i] );
    
    // another way to process command-line args. Tests for null
    //   terminator    
    for ( i = 0; argv[i] != '\0'; i++ )
        printf("%s\n", argv[i] );
        
    // another example -- processes in reverse order
    while( argc-- )
        printf("%s\n", argv[argc] );
        
    
    // let's read data from stdin and process it.
    char buf[80];   // save room for '\0'
    
    printf("Enter your name: " );
    scanf("%s", buf );      // reads a line from stdin
    printf("Enter your age: " );
    scanf("%d", &i );
    printf( "Hello, %s. You are %d years old.\n", buf, i );

    // another example
    float op1, op2, answer;
    char operator;
    printf( "Enter a mathematical expression: " );
    scanf( "%f %c %f", &op1, &operator, &op2 );
    
    switch( operator ) 
    {
        case '*':
            // typecasting the product to avoid compiler warnings
            printf("=%f\n", (float)( op1 * op2) );
        break;
        case '/':
            // display 10 characters, of which 8 are decimal places
            printf("=%10.8f\n", (float)( op1 / op2 ) ); 
        break;
        case '+':
            printf("=%f\n", (float)( op1 + op2 ) );
        break;
        case '-':
            printf("=%f\n", (float)( op1 - op2 ) );
        break; 
        default:
            printf("invalid operation.\n");              
    }




    return 0;
}
