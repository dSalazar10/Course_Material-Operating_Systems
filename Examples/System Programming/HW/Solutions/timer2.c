#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<termios.h>
#include<unistd.h>

#define true 1
#define false -1

void handle_sigint(int);
void handle_sigquit(int);
void handle_echo(int);

int paws = false;

int main(int ac, char* args[])
{    
    // install signal handlers
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);

    // turn echo off
    handle_echo(0);
 
    // convert argument to int
    int seconds = atoi(args[1]);
 
    while(seconds >= 0)
    {
        // spinlock
        while(paws == true)
        {}
    
        printf("%i\n", seconds--);
        sleep(1);
    }
    
    // turn echo on
    handle_echo(1);
}

void handle_echo(int set)
{
    struct termios settings;
    
    tcgetattr(0, &settings);
    
    // turn echo off
    if(set == 0)  
        settings.c_lflag &= ~ECHO;
    // turn echo on
    else    
        settings.c_lflag |= ECHO;
    
    tcsetattr(0, TCSANOW, &settings);
}

void handle_sigint(int p )
{
    paws *= -1;
}

void handle_sigquit(int p)
{
    handle_echo(1);
    exit(0);
}
