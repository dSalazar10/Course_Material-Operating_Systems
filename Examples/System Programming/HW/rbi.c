//
//  rbi.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/24/18.
//
//  This program removes directory entries by inode number
//

#include <sys/stat.h>   /* stat */
#include <stdio.h>      /* perror */
#include <errno.h>      /* errno */
#include <stdbool.h>    /* bool */
#include <unistd.h>     /* getcwd */
#include <sys/dir.h>    /* DIR */
#include <string.h>     /* strlen */
#include <ctype.h>      /* isdigit */

const int MAX = 128;
#define STD_OUT 1

// print how to format arguments
void usage()
{
    char *u = "Usage: rbi INODE\n";
    write(STD_OUT, u, strlen(u));
}
// convert argv[1] into an inode
ino_t convert(char source[])
{
    ino_t result = 0;
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
// convert a path into an inode
ino_t getInode(char* path)
{
    struct stat info;
    if (stat(path, &info) == -1)
    {
        perror(path);
        return -1;
    }
    return info.st_ino;
}
// open a directory by path of dir
DIR* openDir(char path[])
{
    DIR *dirp;
    if ((dirp = opendir(path)) == NULL)
    {
        perror("opendir");
        return NULL;
    }
    return dirp;
}
// removes a directory by inode
bool removeDir(ino_t key)
{
    char path[MAX];
    getcwd(path, sizeof(path));
    DIR *dirp = openDir(path);
    struct dirent *dp;
    ino_t inode;
    
    while((dp = readdir(dirp)) != NULL)
    {
        if(((inode = getInode(dp->d_name)) != -1) && (inode == key))
        {
            remove(dp->d_name);
            closedir(dirp);
            return true;
        }
    }
    if (errno != 0)
        perror("readdir");
    closedir(dirp);
    return false;
}
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        usage();
        return -1;
    }
    if(removeDir(convert(argv[1])) == false)
    {
        char *fail = "Failed to remove directory entry.\n";
        write(STD_OUT, fail, strlen(fail));
    }
    return 0;
}
