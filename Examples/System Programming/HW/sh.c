//
//  sh.c
//  programs
//
//  Created by DANIEL SALAZAR on 5/20/18.
//
// This is a simple version of a shell program.
// it supports cd as well as executes shell functions
// specified by the user. If the user enters "exit",
// the program stops. It also ignores Ctrl-C signals.

#include <stdio.h>      // printf, fgets, stdin, perror
#include <string.h>     // strlen, strcmp
#include <sys/wait.h>   // watpid
#include <unistd.h>     // execlp, fork
#include <signal.h>     // signal
#include <stdlib.h>     // getenv

// a fun separation function
void sep()
{
    char host[255], path[255], user[255];
    gethostname(host, 255);
    getcwd(path, sizeof(path));
    strcpy(user, getenv("USER"));
    printf("________________________________________________________________________________\n| %s @ %s (%s)\n| => ",path, host, user);
}
// skip signals like ^C
void sig_int(int signo)
{
    printf("\n");
    sep();
    printf("\n");
}
int parse(char* input, char* args[])
{
    int i = 0;
    input[strlen(input)-1] = '\0';
    args[i] = strtok(input, " ");
    while((args[++i] = strtok(NULL, " ")));
    return i;
}
int main(int argc, char * argv[])
{
    if(argc != 1)
    {
        printf("usage: %s\n",argv[0]);
        return -1;
    }
    char buf[255], *args[255], *ptr;
    pid_t pid;
    int status, i;
    
    // install signals
    if(signal(SIGINT, sig_int) == SIG_ERR)
        perror("signal");
    
    // priming reed
    sep();
    ptr = fgets(buf, 255, stdin);
    i = parse(buf, args);
    
    // while there is input from stdin and it isn't exit
    while(ptr != NULL && strcmp(buf, "exit") != 0)
    {
        // if input is cd we need to modify the parent cwd
        if(strcmp(args[0],"cd") == 0)
        {
            char *path = (i > 1) ? (args[1]) : getenv("HOME");
            if(chdir(path) != 0)
                perror("cd");
        }
        else
        {
            // fork
            if((pid = fork()) < 0)
            {
                perror("fork");
                return -2;
            }
            //  if parent process, wait
            if(pid != 0)
            {
                pid = wait(&status);
            }
            else
            {
                // execute a command
                if((execvp(args[0], args)) < 0)
                {
                    perror("execvp");
                    return -3;
                }
            }
        }
        
        // continuing reed
        sep();
        ptr = fgets(buf, 255, stdin);
        i = parse(buf, args);
    }
    return 0;
}
