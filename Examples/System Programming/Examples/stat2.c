#include<stdio.h>
#include<pwd.h>
#include<sys/stat.h>
#include<grp.h>
#include<time.h>
#include<string.h>

void mode_to_letters( int, char* );

int main( int ac, char** av )
{
    struct stat info;
    struct passwd;
    
    char perms[10];
    
    if( ac > 1 )
    {
        if ( stat( av[1], & info ) != -1 )
        {
            printf("name: %s\n", av[1] );
            printf("size: %d\n", (int)info.st_size );
            printf("modtime: %s", ctime( &info.st_mtime ) );
            printf("user: %s\n", getpwuid( info.st_uid )->pw_name );
            printf("group: %s\n", getgrgid( info.st_gid )->gr_name );
            printf("links: %d\n", (int)info.st_nlink);
            mode_to_letters( info.st_mode, perms );
            printf("perms: %s\n", perms );        
        }
    
    }
    else
        perror( av[1] );   

    return 1;
}

void mode_to_letters( int mode, char* str )
{
    strcpy( str, "----------" );   
    if( S_ISDIR( mode ) ) str[0] = 'd';
    if( S_ISCHR( mode ) ) str[0] = 'c';
    if( S_ISBLK( mode ) ) str[0] = 'b';
    
    if( mode & S_IRUSR ) str[1] = 'r';    
    if( mode & S_IWUSR ) str[2] = 'w';
    if( mode & S_IXUSR ) str[3] = 'x';

    if( mode & S_IRGRP ) str[4] = 'r';    
    if( mode & S_IWGRP ) str[5] = 'w';
    if( mode & S_IXGRP ) str[6] = 'x';

    if( mode & S_IROTH ) str[7] = 'r';    
    if( mode & S_IWOTH ) str[8] = 'w';
    if( mode & S_IXOTH ) str[9] = 'x';
}

