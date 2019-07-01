//
//  ch1.c
//  programs
//
//  Created by DANIEL SALAZAR on 5/27/18.
//
//  Functions: printf, scanf, fgets, malloc, free,

#include <stdio.h>      // printf, scanf, fgets
#include <stdlib.h>     // malloc, free

// write output according to a format to stdout
// return the number of characters printed if OK, negative value on error
int Printf(char *format)
{
    /* format is as follows:
     1) ordinary chars: (not %), which are copied unchanged to the output stream
     2) conversion specs: (%) each of which results in fetching zero or more subsequent arguments
     the arguments must correspond properly with the conversion specifier
     
     the conversion specs are as follows:
     1) An optional field, consisting of a decimal digit string followed by a $
     2) Zero or more of the following flags:
        a) '#': The value should be converted to an "alternte form"
        b) '0': Zero padding
        c) '-': A negative field width flag
        d) ' ': A blank should be left before a positive number produced by a signed conversion
        e) '+': A sign must always be placed before a number produced by a signed conversion
        f) ''': Decimal conversions or the integral portion of a floating point conversion should
                be grouped and separated by thousands
     3) An optional decimal digit string specifying a minimum field width
     4) An optional precision, in the form of a period . followed by an optional digit string
     5) An optional length modifier, that specifies the size of the argument
     6) A character that specifies the type of conversion to be applied
     
     Spec   Output                              Example
     d/i    Signed decimal integer              392
     u      Unsigned decimal integer            7235
     o      Unsigned octal                      610
     x      Unsigned hexadecimal integer        7fa
     X      Unsigned hexadecimal integer        7FA
     f      Decimal floating point              392.65
     F      Decimal floating point,             392.65
     e      Scientific notation,                3.9265e+2
     E      Scientific notation,                3.9265E+2
     g      Use the shortest representation:    392.65
     G      Use the shortest representation:    392.65
     a      Hexadecimal floating point,         -0xc.90fep-2
     A      Hexadecimal floating point,         -0XC.90FEP-2
     c      Character                           a
     s      String of characters                sample
     p      Pointer address                     b8000000
     n      Store number of chars written
     %      A % followed by another % character %
            will write a single % to the stream
     */
    int results;
    if((results = printf(format)) < 0)
        perror("Error - printf");
    return results;
}

// scans input according to a format
// returns the number of input items assigned if OK, EOF on error
int Scanf(char *format, char buf[])
{
    /* format is as follows:
     1) Whitespace character: read and ignore any whitespace characters
        encountered before the next non-whitespace character
     2) Non-whitespace character: (not %) read the next character
        from the stream, compare it to this non-whitespace character
        and if it matches, it is discarded and the function continues
        with the next character of format
     3) Format specifiers: (%) specify the type and format of the data
        to be retrieved from the stream and stored into the locations
        pointed by the additional arguments
     
     format specs are as follows:
     1) i (Integer):
        a) Any number of digits, optionally preceded by a sign (+ or -).
        b) Decimal digits assumed by default (0-9), but a 0 prefix introduces
           octal digits (0-7), and 0x hexadecimal digits (0-f).
        c) Signed argument.
     2) d/u (Decimal integer):
        a) Any number of decimal digits (0-9), optionally preceded by a sign (+ or -).
        b) d is for a signed argument, and u for an unsigned.
     3) o (Octal/Integer):
        a) Any number of octal digits (0-7), optionally preceded by a sign (+ or -).
        b) Unsigned argument.
     4) x (Hexadecimal/Integer):
        a) Any number of hexadecimal digits (0-9, a-f, A-F), optionally preceded
           by 0x or 0X, and all optionally preceded by a sign (+ or -).
        b) Unsigned argument.
     5) f/e/g/a (Float):
        a) A series of decimal digits, optionally containing a decimal point,
           optionally preceeded by a sign (+ or -) and optionally followed by
           the e or E character and a decimal integer (or some of the other
           sequences supported by strtod).
        b) Implementations complying with C99 also support hexadecimal
           floating-point format when preceded by 0x or 0X.
     6) c (Character):
        a) The next character. If a width other than 1 is specified, the function
           reads exactly width characters and stores them in the successive locations
           of the array passed as argument. No null character is appended at the end.
     7) s (C-String):
        a) Any number of non-whitespace characters, stopping at the first whitespace
           character found. A terminating null character is automatically added at
           the end of the stored sequence.
     8) p (Pointer):
        a) A sequence of characters representing a pointer. The particular format used
           depends on the system and library implementation, but it is the same as the
           one used to format %p in fprintf.
     9) [characters] (Scanset):
        a) Any number of the characters specified between the brackets.
        b) A dash (-) that is not the first character may produce non-portable behavior
           in some library implementations.
     10) [^characters] (Negated Scanset):
        a) Any number of characters none of them specified as characters between the brackets.
     11) n (Count):
        a) No input is consumed.
        b)The number of characters read so far from stdin is stored in the pointed location.
     12) % (Percent Sign):
        a) A % followed by another % matches a single %.
     */
    int results;
    if((results = scanf(format, buf)) == EOF)
        perror("Error - scanf");
    return results;
}

// reads n-1 characters from the given stream, fp, and stores them in buf.
// returns buf if OK, NULL on EOF, NULL on error
char* Fgets(char *buf, int n, FILE* fp)
{
    /*
     The buf variable is always NULL terminated.
     If the line is longer than n-1, only a partial line is returned, and a
     second call to fgets will read what follows on the line.
     */
    int *results;
    if((results = fgets(buf, n, fp)) == NULL)
        perror("Error - fgets");
    return results;
}

// allocates size bytes of memory
// returns a non-null ptr of OK, NULL on error
void* Malloc(size_t size)
{
    void * results;
    if((results = malloc(size)) == NULL)
        perror("Error - malloc");
    return results;
}

// deallocates the memory allocation pointed to by ptr
void Free(void *ptr)
{
    // If ptr is a NULL pointer, no operation is performed
    free(ptr);
    return;
}


int main(int argc, char *argv[])
{
    printf("Basic functions - skip the examples please...");
    return 0;
}
