//
//  confstr.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/7/18.
//
// get configuration defined string values

#include <stdio.h>
#include <stdlib.h>     /* malloc */
#include <unistd.h>     /* confstr */
#include <errno.h>      /* errno */

// system variables
#define userdir _CS_DARWIN_USER_DIR
#define tempdir _CS_DARWIN_USER_TEMP_DIR
#define cachedir _CS_DARWIN_USER_CACHE_DIR
#define path _CS_PATH

int get_config(int p)
{
    char *pathbuf;
    size_t n;
    
    n = confstr(p, NULL, (size_t) 0);
    pathbuf = malloc(n);
    if(pathbuf == NULL || (n = confstr(p, pathbuf, n)) == 0)
    {
        if(errno == EINVAL)
            perror("Error");
        else if(errno == ENOMEM)
            perror("Error");
        else if(errno == EIO)
            perror("Error");
        else
            printf("Error with confstr.\n");
        return -1;
    }
    else
    {
        printf("\nPath: %s\n", pathbuf);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
        printf("Usage: confstr DIR_NUMBER\n1)userdir\n2)tempdir\n3)cachedir\n4)PATH\n");
    else
    {
        int p;
        switch(*argv[1])
        {
            case '1': p = userdir;
                break;
            case'2': p = tempdir;
                break;
            case 3: p = cachedir;
                break;
            case 4: p = path;
        }
        get_config(p);
    }
    return 0;
}
