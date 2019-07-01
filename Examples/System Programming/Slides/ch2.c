//
//  ch2.c
//  programs
//
//  Created by DANIEL SALAZAR on 5/26/18.
//
//  Functions: perror, open, creat, close, lseek, read, write,

#include <stdio.h>      // printf
#include <fcntl.h>      // open, creat
#include <unistd.h>     // close, lseek, read, write

// open or create a file for reading or writing
// returns file descriptor if OK, -1 on error
int Open(char *path, int oflag)
{
    /* oflag is the following:
     O_RDONLY        open for reading only
     O_WRONLY        open for writing only
     O_RDWR          open for reading and writing
     O_NONBLOCK      do not block on open or for data to become available
     O_APPEND        append on each write
     O_CREAT         create file if it does not exist
     O_TRUNC         truncate size to 0
     O_EXCL          error if O_CREAT and the file exists
     O_SHLOCK        atomically obtain a shared lock
     O_EXLOCK        atomically obtain an exclusive lock
     O_NOFOLLOW      do not follow symlinks
     O_SYMLINK       allow open of symlinks
     O_EVTONLY       descriptor requested for event notifications only
     O_CLOEXEC       mark as close-on-exec
     */
    int results;
    if((results = open(path, oflag)) == -1)
        perror("Error - open");
    return results;
}

// create a new file
// returns file descriptor opened for write-only if OK, -1 on error
int Creat(char *path, mode_t mode)
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
     
     This is the same as the following open call:
     open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
     
     The drawback to creat is that the file is opened
     only for writing. You used to have to make three
     function calls to make a temp file and read it:
     creat, close, open.
     
     Now you could just use open:
     open(path, O_RDWR | O_CREAT | O_TRUNC, mode);
    */
    int results;
    if((results = creat(path, mode)) == -1)
        perror("Error - creat");
    return results;
}

// close an open file
// returns 0 if OK, -1 if error
int Close(int fd)
{
    /*
     closing a file also releases any record lock
     that the process may have on the file. So you
     could be lazy and skip closing a file, if it
     is opened by a process you know will be killed.
     */
    int results;
    if((results = close(fd)) == -1)
        perror("Error - close");
    return results;
}

// explicitly set an open file's offset
// returns file's new offset if OK, -1 on error
off_t Lseek(int fd, off_t offset, int whence)
{
    /* whence is the following:
     SEEK_SET,  the offset is set to offset bytes
     SEEK_CUR,  the offset is set to its current
                location plus offset bytes
     SEEK_END,  the offset is set to the size of
                the file plus offset bytes
     
     We could determine the current offset by seeking
     zero bytes:
     off_t currpos = lseek(fd, 0, SEEK_CUR);
     */
    int results;
    if((results = lseek(fd, offset, whence)) == -1)
        perror("Error - lseek");
    return results;
}

// read is a builtin system call that reads data from an open file
// returns the number of bytes read if OK, 0 on EOF, -1 on error
ssize_t Read(int fd, void *buf, size_t nbytes)
{
    /*
     The amount read could be less than requested if
     something like a signal interrupts the read or if
     reading from a terminal device, since only one line
     is read at a time.
     
     The read begins at the current offset of the file.
     */
    int results;
    if((results = read(fd, buf, nbytes)) == -1)
        perror("Error - read");
    else if(results == 0)
        printf("EOF\n");
    return results;
}

// write data to an open file
// returns number of bytes written if OK, -1 on error
ssize_t Write(int fd, void *buf, size_t nbytes)
{
    /*
     A common error is if disk is full or exceeding the
     filesize for a give process.
     
     The write begins at the current offset of the file.
     If the O_APPEND option was set when the file was opened,
     the file's offset is set to the current end of the file
     before each write execution. After the execution, the
     offset will be incremented by the number of bytes actually
     written.
     */
    int results;
    if((results = write(fd, buf, nbytes)) == -1)
        perror("Error - write");
    return results;
}

/*    Examples    */

// create a file with a whole in it
void Hole()
{
    char buf1[] = "abcdefghij";
    char buf2[] = "ABCDEFGHIJ";
    int fd;
    
    // create the file RW permission for user, R permission for group and others
    if((fd = creat("file.hole", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
        perror("Error - creat");
    
    if(write(fd, buf1, 10) != 10)
        perror("Error - write");
    /* the offset is now 10 */
    
    if(lseek(fd, 16384, SEEK_SET) == -1)
        perror("Error - lseek");
    /* the offset is now 16384 */
    
    if(write(fd, buf2, 10) != 10)
        perror("Error - write");
    /* the offset is now 16394 */
    
    return;
}

// copy stdin to stdout using read and write
void Copy()
{
    int n;
    char buf[4096];
    
    while((n = read(STDIN_FILENO, buf, 4096)) > 0)
    {
        if(write(STDOUT_FILENO, buf, n) != n)
            perror("Error - write");
    }
    
    if(n < 0)
        perror("Error - read");
    
    return;
}

int main(int argc, char *argv[])
{
    Copy();
    return 0;
}



