//
//  ls.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/12/18.
//


#include <stdio.h>      /* printf, perror */
#include <stdlib.h>     /* malloc, free, exit */
#include <string.h>     /* strlen, strcmp, strcpy */
#include <sys/queue.h>  /* tailq */
#include <stdbool.h>    /* bool */
#include <time.h>       /* ctime */
#include <grp.h>        /* getgrgid */
#include <sys/dir.h>    /* opendir, readdir, closedir, dirent */
#include <errno.h>      /* errno */
#include <sys/stat.h>   /* stat */
#include <sys/ioctl.h>  /* ioctl, winsize, TIOCGWINSZ */
#include <unistd.h>     /* getopt, getcwd, STDOUT_FILENO */
#include <pwd.h>        /* getpwuid */

// Testing process info stuff
// from  /usr/include
#include <libproc.h>

const int MAX = 1024;
static int col_width = 0, col_height = 0;

#define max(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })

// Display Flags
static bool Aflag, Rflag, aflag, lflag, rflag, numflag;

/* List Definitions*/
struct fileinfo
{
    
    /* The name of the file  */
    char name[MAX];
    /* The information about the file */
    struct stat stat;
    /* A reference (shortcut) to a file */
    char * links;
};
typedef struct node
{
    struct fileinfo fileinfo;
    TAILQ_ENTRY(node) nodes;
} node_t;
typedef TAILQ_HEAD(head_s, node) list;

/* List Prototypes */
bool IsInList(list * head, const char * string);
static struct node* getListEl();
static void AddEl(list * head, struct fileinfo * fileinfo);
char* RemoveEl(list * head, const char * string);
static void clear(list * head);
int getLines();
int getCols();
static int GetLength(list * head);
void bubbleSort(list * head, bool reverse);
void Sort(list * head, bool reverse);
static void Print(list * head);
static void print(list * head);

/* List Functions */
bool IsInList(list * head, const char * string)
{
    struct node * e = NULL;
    TAILQ_FOREACH(e, head, nodes)
    {
        if(strcmp(e->fileinfo.name, string) == 0)
        {
            return true;
        }
    }
    return false;
}
static struct node* getListEl()
{
    struct node * e = malloc(sizeof(struct node));
    if (e == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return e;
}
static void AddEl(list * head, struct fileinfo * fileinfo)
{
    if(IsInList(head, fileinfo->name)) return;
    struct node * e = getListEl();
    int i, len = strlen(fileinfo->name);
    for(i = 0; i < len; ++i)
    {
        e->fileinfo.name[i] = fileinfo->name[i];
        e->fileinfo.stat = fileinfo->stat;
    }
    TAILQ_INSERT_TAIL(head, e, nodes);
    col_width = max(col_width, len);
    e = NULL;
}
char* RemoveEl(list * head, const char * string)
{
    struct node *e = NULL;
    char *data;
    TAILQ_FOREACH(e, head, nodes)
    {
        if (strcmp(e->fileinfo.name, string) == 0)
        {
            data = malloc(sizeof(e->fileinfo.name));
            strcpy(data, e->fileinfo.name);
            TAILQ_REMOVE(head, e, nodes);
            free(e);
            return data;
        }
    }
    return NULL;
}
static void clear(list * head)
{
    struct node * e = NULL;
    while (!TAILQ_EMPTY(head))
    {
        e = TAILQ_FIRST(head);
        TAILQ_REMOVE(head, e, nodes);
        free(e);
        e = NULL;
    }
}
// terminal height
int getLines()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}
// terminal width
int getCols()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}
// sorting
static int GetLength(list * head)
{
    int len = 0;
    struct node * e = NULL;
    TAILQ_FOREACH(e, head, nodes)
    {
        len++;
    }
    return len;
}
void bubbleSort(list * head, bool reverse)
{
    struct node *ie = NULL, *je = NULL;
    int i, j, n = GetLength(head);
    for (i = 0; i < n-1; i++)
    {
        for (j = 0, ie = TAILQ_FIRST(head); j < n-i-1; j++, ie = TAILQ_NEXT(ie, nodes))
        {
            je = TAILQ_NEXT(ie, nodes);
            if(reverse == true)
            {
                if (strcmp(ie->fileinfo.name,je->fileinfo.name) < 0)
                {
                    char temp[MAX] = "";
                    strcpy(temp, ie->fileinfo.name);
                    strcpy(ie->fileinfo.name, je->fileinfo.name);
                    strcpy(je->fileinfo.name, temp);
                }
            }
            else
            {
                if (strcmp(ie->fileinfo.name,je->fileinfo.name) > 0)
                {
                    char temp[MAX] = "";
                    strcpy(temp, ie->fileinfo.name);
                    strcpy(ie->fileinfo.name, je->fileinfo.name);
                    strcpy(je->fileinfo.name, temp);
                }
            }
            
        }
    }
}
void Sort(list * head, bool reverse)
{
    bubbleSort(head, reverse);
}
// printing
void printSpaces(int sp)
{
    int i;
    for(i = 0; i < sp; i++)
        printf(" ");
}
static void Print(list * head)
{
    struct node * e = NULL;
    char buf[20];
    if(lflag == true || numflag == true)
    { // print one element per line
        TAILQ_FOREACH(e, head, nodes)
        {
            strmode(e->fileinfo.stat.st_mode, buf);
            printf("%s ", buf);
            printf("%d\t", e->fileinfo.stat.st_nlink);
            printf("%s\t", getpwuid(e->fileinfo.stat.st_uid)->pw_name);
            printf("%s\t", getgrgid(e->fileinfo.stat.st_gid)->gr_name);
            printf("%lld\t", e->fileinfo.stat.st_size);
            time_t time = (time_t)((e->fileinfo.stat.st_mtimespec).tv_sec);
            printf("%12.16s  ", ctime(&time));
            printf("%s\n", e->fileinfo.name);
        }
    }
    else
    { // print table
        unsigned short col = 0;
        int num = getCols()/(col_width + 2);
        TAILQ_FOREACH(e, head, nodes)
        {
            if(col_width > (getCols() - col))
            {
                printf("\n");
                col = 0;
            }
            printf("%s", e->fileinfo.name);
            printSpaces(col_width - strlen(e->fileinfo.name));
            col += col_width + 2;
            printf("  ");
        }
        printf("\n");
    }
}
static void print(list * head)
{
    if(rflag == true)
    {
        Sort(head, true);
    }
    else
    {
        Sort(head, false);
    }
    Print(head);
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
void procpid(pid_t pid)
{
    struct proc_bsdinfo proc;
    int st = proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc, PROC_PIDTBSDINFO_SIZE);
    printf("name: %s\n", proc.pbi_name);
}
void pidlist()
{
    int bufsize = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    pid_t pids[2 * bufsize / sizeof(pid_t)];
    bufsize = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    size_t num_pids = bufsize / sizeof(pid_t);
    int i = 0;
    while (i < num_pids)
    {
        printf("pid[%d]::%d\n", i, pids[i]);
        procpid(pids[i]);
        printf("\n-------------------------------\n\n");
        i += 1;
    }
}
void test()
{
    pidlist();
}
int main(int argc, char *argv[])
{
    
    /*
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
     */
    test();
}

