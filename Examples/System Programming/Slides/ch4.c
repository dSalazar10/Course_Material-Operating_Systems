//
//  ch4.c
//  programs
//
//  Created by DANIEL SALAZAR on 5/26/18.
//
//  Functions: mkdir, rmdir, chdir, link, unlink, symlink

#include <stdio.h>      // printf
#include <sys/stat.h>   // mkdir
#include <unistd.h>     // rmdir, chdir, link, unlink, symlink

// create a new, empty directory with the access permissions specified by mode
// returns 0 if OK, -1 on error
int Mkdir(char *path, mode_t mode)
{
    /*mode_t is the following:
     #define S_IRWXU 0000700     RWX mask for owner
     #define S_IRUSR 0000400     R for owner
     #define S_IWUSR 0000200     W for owner
     #define S_IXUSR 0000100     X for owner
     
     #define S_IRWXG 0000070     RWX mask for group
     #define S_IRGRP 0000040     R for group
     #define S_IWGRP 0000020     W for group
     #define S_IXGRP 0000010     X for group
     
     #define S_IRWXO 0000007     RWX mask for other
     #define S_IROTH 0000004     R for other
     #define S_IWOTH 0000002     W for other
     #define S_IXOTH 0000001     X for other
     
     #define S_ISUID 0004000     set user id on execution
     #define S_ISGID 0002000     set group id on execution
     #define S_ISVTX 0001000     save swapped text even after use
     */
    int results;
    if((results = mkdir(path, mode)) == -1)
        perror("Error - mkdir");
    return results;
}

// removes a directory file whose name is given by path
// returns 0 if OK, -1 on error
int Rmdir(char *path)
{
    /*
     If the link of the directory becomes 0 with this call,
     and if no other process has this directory open, then
     the space occupied by the directory is freed. If a
     process has this directory open, the last link, dot,
     and dot-dot are removed before this function returns.
     Until the processes close this directory, no new files
     can be created in it. The directory is not freed until
     the last process closes it.
     */
    int results;
    if((results = rmdir(path)) == -1)
        perror("Error - rmdir");
    return results;
}

// causes the named directory to become the current working directory
// returns 0 if OK, -1 on error
int Chdir(char *path)
{
    int results;
    if((results = chdir(path)) == -1)
        perror("Error - chdir");
    return results;
}

// atomically creates the specified directory entry (hard link)
// path2 with the attributes of the underlying object pointed at by path1
// returns 0 if OK, -1 on error
int Link(char *path1, char *path2)
{
    /*
     If the link is successful, the link count of the underlying object is incremented
     If path1 is removed and path2 ins't, the link count of the underlying object is decremented
     
     In order for the system call to succeed, path1 must exist
     and both path1 and path2 must be in the same file system
     */
    int results;
    if((results = link(path1, path2)) == -1)
        perror("Error - link");
    return results;
}

// removes the link named by path from its directory and decrements
// the link count of the file which was referenced by the link
// returns 0 if OK, -1 on error
int Unlink(char *path)
{
    /*
     If that decrement reduces the link count of the file to zero,
     and no process has the file open, then all resources associated
     with the file are reclaimed.
     
     If one or more process have the file open when the last link is
     removed, the link is removed, but the removal of the file is
     delayed until all references to it have been closed.
     */
    int results;
    if((results = unlink(path)) == -1)
        perror("Error - unlink");
    return results;
}

// A symbolic link path2 is created to path1
// returns 0 if OK, -1 on error
int Symlink(char *path1, char *path2)
{
    int results;
    if((results = symlink(path1, path2)) == -1)
        perror("Error - symlink");
    return results;
}

/*    Example    */

#include<fcntl.h>
void UnlinkExample(char *path)
{
    if(open(path, O_RDWR) == -1)
        perror("Error - open");
    if(unlink(path) == -1)
        perror("unlink");
    printf("%s unlinked \n", path);
    return;
}

int main(int argc, char *argv[])
{
    if(argc < 2)
        printf("usage: %s filename", argv[0]);
    else
        UnlinkExample(argv[1]);
    return 0;
}
