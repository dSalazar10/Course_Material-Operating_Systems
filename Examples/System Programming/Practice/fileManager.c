//
//  fileManager.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/12/18.
//


#include <stdio.h>
#include <stdbool.h>    /* bool */
#include <stdlib.h>     /* getenv */
#include <sys/dir.h>    /* dirent */
#include <errno.h>      /* perror */
#include <string.h>     /* strcmp */
#include "queue.h"      /* linkedlist */
#include <sys/stat.h>   /* mkdir */
#include <getopt.h>     /* getopt */
#include <unistd.h>     /* getcwd */
/* sysctl*/
#include <sys/types.h>
#include <sys/sysctl.h>
/* getpwuid*/
#include <pwd.h>
#include <uuid/uuid.h>


/* Discovering Directory Contents */
char* tempDir()
{
    //Returns the temporary directory for the current user.
    return getenv("TMPDIR");
}
char* homeDir()
{
    //Returns the home directory for the current user.
    return getenv("HOME");
}
DIR* openDir(char path[])
{
    // opens the directory named by path, associates
    // a directory stream with it
    DIR *dirp;
    if ((dirp = opendir(path)) == NULL) {
        // The pointer NULL is returned if filename cannot
        // be accessed, or if it cannot create enough
        // memory to hold the whole thing, and sets the
        // global variable errno to indicate the error.
        perror("opendir");
        return NULL;
    }
    // returns a pointer to be used to identify the directory stream
    return dirp;
}
list contentsOfDir(char path[])
{
    //Performs a shallow search of the specified directory and returns the paths of any contained items.
    DIR *dirp = openDir(path);
    struct dirent *dp;
    list head;
    TAILQ_INIT(&head);
    while((dp = readdir(dirp)) != NULL)
    {
        if((Aflag == true && (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)))
            continue;
        else if(aflag == false && *(dp->d_name) == '.')
            if(Aflag == false)
                continue;
        
        struct fileinfo *fileinfo = malloc(sizeof(struct fileinfo));
        strcpy(fileinfo->name, dp->d_name);
        
        char source[MAX] = "";
        strcpy(source, path);
        strcat(source, "/");
        strcat(source, dp->d_name);
        stat(source, &(fileinfo->stat));
                
        AddEl(&head, fileinfo);
    }
    if (errno != 0)
        perror("readdir");
    closedir(dirp);
    return head;
}
char* subpathsOfDir(char path[])
{
    //Returns an array of strings identifying the paths for all items in the specified directory.
    // This method recurses the specified directory and its subdirectories. The method skips the
    // “.” and “..” directories at each level of the recursion.
    return NULL;
}

/* Creating and Deleting Items */
bool createDir(char path[], char attr[])
{
    //Creates a directory with the given attributes at the specified path
    return false;
}
bool createFile(char path[], char data[], char attr[])
{
    //Creates a file with the specified content and attributes at the given location
    return false;
}
bool removeItem(char path[])
{
    //Removes the file or directory at the specified path.
    return false;
}

/* Moving and Copying Items */
bool moveItem(char src[], char dst[])
{
    //Moves the file or directory at the specified path to a new location
    //Delete src item
    return false;
}
bool copyItem(char src[], char dst[])
{
    //Copies the item at the specified path to a new location.
    //Duplicate src item
    return false;
}

/* Creating Symbolic and Hard Links */
bool createSymbolicLink(char src[], char dst[])
{
    //Creates a symbolic link that points to the specified destination.
    return false;
}
bool linkItem(char src[], char dst[])
{
    //Creates a hard link between the items at the specified paths.
    return false;
}
char* destinationOfSymbolicLink(char path[])
{
    //Returns the path of the item pointed to by a symbolic link.
    return NULL;
}


/* Determining Access to Files */
bool fileExists(char path[])
{
    //Returns a Boolean value that indicates whether a file or directory exists at a specified path.
    DIR *dirp;
    return ((dirp = opendir(path)) == NULL) ? (false) : (true);
}
bool isReadableFile(char path[])
{
    //Returns a Boolean value that indicates whether the invoking object appears able to read a specified file.
    return false;
}
bool isWritableFile(char path[])
{
    //Returns a Boolean value that indicates whether the invoking object appears able to write to a specified file.
    return false;
}
bool isExecutableFile(char path[])
{
    //Returns a Boolean value that indicates whether the operating system appears able to execute a specified file.
    return false;

}
bool isDeletableFile(char path[])
{
    //Returns a Boolean value that indicates whether the invoking object appears able to delete a specified file.
    return false;

}

/* Getting and Setting Attributes */
char* componentsToDisplay(char path[])
{
    //Returns an array of strings representing the user-visible components of a given path.
    return NULL;
}
char* displayName(char path[])
{
    //Returns the display name of the file or directory at a specified path.
    return NULL;
}
char* attributesOfItem(char path[])
{
    //Returns the attributes of the item at a given path.
    return NULL;
}
char* attributesOfFileSystem(char path[])
{
    //Returns a dictionary that describes the attributes of the mounted file system on which a given path resides.
    return NULL;
}
bool setAttributes(char path[], char attr[])
{
    //Sets the attributes of the specified file or directory.
    return false;

}

/* Getting and Comparing File Contents */
char* contents(char path[])
{
    //Returns the contents of the file at the specified path.
    return NULL;
}
bool contentsEqual(char src[], char dst[])
{
    //Returns a Boolean value that indicates whether the files or directories in specified paths have the same contents.
    return false;
}

void test()
{
    
}

int main(int argc, char *argv[])
{
    int c;
    char path[MAX];
    while ((c = getopt(argc, argv, "ARalr1")) != -1)
    {
        switch (c)
        {
            case 'A': // almost-all
                Aflag = true;
                break;
            case 'R': // recursive
                Rflag = true;
                break;
            case 'a': // all
                aflag = true;
                break;
            case 'l': // long
                lflag = true;
                break;
            case 'r': // reverse
                rflag = true;
                break;
            case '1': // one entry per line
                numflag = true;
                break;
            default:
                printf("usage: %s [-ABFGLRTabdhinqrsw1]\n", argv[0]);
                break;
        }
    }
    list head;
    TAILQ_INIT(&head);
    getcwd(path, sizeof(path));
    head = contentsOfDir(path);
    print(&head);
    return 0;
}


// directory operations
// opendir(3), fdopendir(3), readdir(3), readdir_r(3),
// telldir(3), seekdir(3), rewinddir(3), closedir(3),
// dirfd(3)









