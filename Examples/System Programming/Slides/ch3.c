//
//  ch3.c
//  programs
//
//  Created by DANIEL SALAZAR on 5/26/18.
//
//  Functions: opendir, readdir, closedir, seekdir, stat, chmod, chown, utimes, rename

#include <stdio.h>      // rename, printf
#include <dirent.h>     // opendir, readdir, closedir, seekdir
#include <sys/stat.h>   // stat, chmod
#include <unistd.h>     // chown
#include <sys/time.h>   // utimes

// opens a directory and associates a filestream with it
// returns a pointer to the stream if OK, NULL on error
DIR * OpenDir(char *path)
{
    DIR *dp;
    if((dp = opendir(path)) == NULL)
        perror("Error - opendir");
    return dp;
}

// reads an open directory
// returns a pointer to the next directory
// in the stream if OK, NULL on error
struct dirent * ReadDir(DIR *dp)
{
    struct dirent *dirp;
    if((dirp = readdir(dp)) == NULL)
        perror("Error - opendir");
    return dirp;
}

// closes an open directory
// returns 0 if OK, -1 on error
int CloseDir(DIR *dp)
{
    int results;
    if((results = closedir(dp)) == -1)
        perror("Error - opendir");
    return results;
}

// sets the position of the next readdir() in the stream
void SeekDir(DIR *dp, long loc)
{
    seekdir(dp, loc);
}

// fills the stat struct with info about the file
// return 0 if OK, -1 on error
int Stat(char *path, struct stat *buf)
{
    /* stat struct is as follows:
     dev_t              st_dev;         device inode resides on
     ino_t              st_ino;         inode's number
     mode_t             st_mode;        inode protection mode
     nlink_t            st_nlink;       number of hard links to the file
     uid_t              st_uid;         user-id of owner
     gid_t              st_gid;         group-id of owner
     dev_t              st_rdev;        device type, for special file inode
     struct timespec    st_atimespec;   time of last access
     struct timespec    st_mtimespec;   time of last data modification
     struct timespec    st_ctimespec;   time of last file status change
     off_t              st_size;        file size, in bytes
     quad_t             st_blocks;      blocks allocated for file
     u_long             st_blksize;     optimal file sys I/O ops blocksize
     u_long             st_flags;       user defined flags for file
     u_long             st_gen;         file generation number
     
     timespec is as follows:
     time_t             tv_sec;
     long               tv_nsec;
     
     */
    int results;
    if((results = stat(path, buf)) == -1)
        perror("Error - stat");
    return results;
}

// sets the specified file's permission to mode's value
// return 0 if OK, -1 on error
int Chmod(char *path, mode_t mode)
{
    /* mode_t is the following:
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
    if((results = chmod(path, mode)) == -1)
        perror("Error - chmod");
    return results;
}

// change a file's UID or GID, if either is -1 then ID is unchanged
// returns 0 if OK, -1 on error
int Chown(char *path, uid_t owner, gid_t group)
{
    int results;
    if((results = chown(path, owner, group)) == -1)
        perror("Error - chown");
    return results;
}

// change the access and modification times of a file
// returns 0 if OK, -1 on error
int Utimes(char *path, struct timeval times[2])
{
    /* timeval is as follows:
     time_t         tv_sec      seconds
     suseconds_t    tv_usec     microseconds
     
     times is an array of two timestamps access/modification time
     found in the struct stat variables st_atimespec/st_mtimespec
     */
    int results;
    if((results = utimes(path, times)) == -1)
        perror("Error - utimes");
    return results;
}

// rename a file, directory, or symbolic link
// this requires write permissions for file/dir/link
// returns 0 if OK, -1 on error
int Rename(char *oldname,char *newname)
{
    /*
     The cases: file, dir, or symlink
     
     1) oldname is not a directory:
     if newname exists, it can't be a dir.
     if newname exists & not a dir, it is removed, then
     oldname is renamed to newname
     
     2) oldname is a directory:
     if newname exists, it must be an empty dir.
     if newname exists & is empty, it is removed.
     if newname is removed, it cannot contain a path prefix that names oldname, then
     oldname is renamed to newname
     
     3) oldname or newname are a symlink:
     the link itself is processed, not the file to which it resolves.
     
     4) dot or dot-dot can't appear as the last component of oldname/newname:
     
     5) oldname/newname are equivalent, nothing happens
     */
    int results;
    if((results = rename(oldname, newname)) == -1)
        perror("Error - rename");
    return results;
}

#include<fcntl.h>
void LseekDemo(char *path)
{
    char buffer[128];
    
    // open file
    int fd = open( path, O_RDONLY );
    
    // move 20 bytes forward within the file
    lseek( fd, 20, SEEK_SET );
    
    // read 128 bytes from the file into buffer
    read( fd, buffer, 128 );
    
    // print contents of buffer
    printf("%s\n", buffer );
    
    // move 20 bytes back in file
    lseek( fd, -20, SEEK_CUR );
    
    // read 128 bytes from the file into buffer
    read( fd, buffer, 128 );
    
    // print contents of buffer
    printf("\n\n\n\n\n\n%s\n", buffer );
    
    close( fd );
}

int main(int argc, char *argv[])
{
    LseekDemo(argv[1]);
    return 0;
}

