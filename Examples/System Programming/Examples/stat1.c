#include<stdio.h>
#include<sys/stat.h>

int main( int ac, char** av )
{
    struct stat info;   // see man 2 stat for list of fields
    
    if( ac > 1 )
    {
        if ( stat( av[1], & info ) != -1 )
        {
            printf("name: %s\n", av[1] );
            printf("size: %d\n", (int)info.st_size );
            printf("modtime: %d\n", (int)info.st_mtime );
            printf("user: %d\n", info.st_uid );
            printf("group: %d\n", info.st_gid );
            return 0;        
        }
    }
    else
        perror( av[1] );   

    return 1;
}
