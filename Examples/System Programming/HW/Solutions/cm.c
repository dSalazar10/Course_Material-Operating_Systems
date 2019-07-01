/*
    Write your own version of a limited chmod system command.
    This command will allow the user to set the permissions
    of a file for the owner of a file only, leaving the permissions
    for the group and other unchanged.
    
    The command should use symbolic representation of the
    permissions.
    
    Synopsis:
    cm FILE [rwx]
    
    A combination of the letters rwx select file mode bits for the 
    the owner of the file:  read (r), write (w), and execute
    ( or search for directories ) (x).
    
    Sample Usages:

    cm FILE r      sets permissions for the owner to read only.
    cm FILE xr     sets permissions for the owner to read and execute only.
    cm FILE        clears all permissions for the user of the file
    cm             displays synopsis message for the command.
*/

#include "stdio.h"
#include "stdlib.h"
#include "sys/stat.h"
#include "sys/types.h"


/*
    1 7   7   0   7   7
    ---- --- --- --- ---
    1111 111 000 111 111

*/

int main( int ac, char* av[] )
{
    struct stat buf;
    int result, i;
    mode_t mode;
 
    // display synopsis, if command invoked with no arguments   
    if ( ac == 1 )
    {
        printf("cm FILE [rwx]\n");
        exit(1);
    }
    
    // retrieve the current properties of the file
    if ( ( result = stat( av[1], &buf ) ) == -1 )
    {
        perror( av[1] );
        exit(2);
    }
    
    mode = buf.st_mode; // store the current mode of the file
    mode &= 0177077;    // clear the rwx bits of the owner of the file
    
    i = 0;
    
    // if bits are specified, update the mode copy as specified
    // at the command line.
    if ( ac == 3 )
    {
        while( av[2][i] != '\0' )
        {
            switch( av[2][i] )
            {
                case 'r':
                    mode = mode | S_IRUSR;
                break;
                case 'w':
                    mode = mode | S_IWUSR;            
                break;
                case 'x':
                    mode = mode | S_IXUSR;            
                break;
            }
            
            i++;
        }
    }    

    // push the updated local copy of the file mode to the file
    if ( ( result = chmod( av[1], mode ) ) == -1 )
    {
        perror( av[1] );
        exit(2);
    }   
    
    return 0;
}
