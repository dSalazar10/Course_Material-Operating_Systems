/*
 calc.c
 
 Daniel Salazar
 March 29 2018
 CS 3560
 HW 1 Due : Wednesday, April 11th, 11:59 PM
 
 This program is a simple calculator to be used as a first example of
 a shell program.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

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

// convert a c-string to a float
float convert(char source[])
{
    float result = 0.0;
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

int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        // Print basic Man Page
        printf("\nNAME:\n\tcalc - a basic calculator.\n\nSYNOPSIS:\n\tcalc OPERATOR ");
        printf("OPERAND1 OPERAND2\n\nDESCRIPTION:\n\tAdds, subtracts, or divides two ");
        printf("numbers.\n\nEXAMPLES:\n ------------------------------------------------\n");
        printf("|./calc\t\t\t|displays synopsis mesage|\n|./calc + 3 2\t\t|displays 5.000000\t ");
        printf("|\n|./calc - 3 2\t\t|displays 1.000000\t |\n|./calc / 3 2\t\t");
        printf("|displays 1.500000\t |\n ------------------------------------------------\n");
    }
    else
    {
        char operator[25] = "", op1[25] = "", op2[25] = "";
        float term1, term2;
        
        // extract command line arguments
        loadOp(argv[1], operator);
        loadOp(argv[2], op1);
        loadOp(argv[3], op2);
        
        // convert args to floats
        term1 = convert(op1);
        term2 = convert(op2);
        
        // perform arithmetic
        if(operator[0] == '+') // add
        {
            printf("%f\n",term1 + term2);
        }
        else if (operator[0] == '-') // subtract
        {
            printf("%f\n",term1 - term2);
        }
        else if (operator[0] == '/') // divide
        {
            printf("%f\n",term1 / term2);
        }
        else // Unsupported operators - do nothing
        {
            printf("\n");
        }
    }
    return 0;
}
