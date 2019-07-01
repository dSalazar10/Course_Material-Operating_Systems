//
//  timer.c
//  programs
//
//  Created by DANIEL SALAZAR on 5/19/18.
//
// "Description
// Modify HW05 to use signals to pause the output of the timer
// instead of control characters.  So, the only terminal setting
// you will need to modify programatically is echoing.   If the
// user hits ctrl-C, the program pauses.  If they hit it again,
// it unpauses.  Ctrl-\ terminates and restores the settings gracefully."

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <signal.h>

int e = 0, p = 0;

void usage(char name[])
{
    printf("Usage: %s [number]\n", name);
}
void quit(int signal)
{
    // this will pause/unpause timer
    if(signal == SIGINT) p = !p;
    // this will end the timer
    else if(signal == SIGQUIT) e = 1;
}
void timer(int time)
{
    // Old and new termios settings
    struct termios oldSettings, newSettings;
    // Get the current attributes
    tcgetattr(fileno(stdin), &oldSettings);
    // copy settings for manipulation
    newSettings = oldSettings;
    // turn input Echo off
    newSettings.c_lflag &= ~ECHO;
    // Set new settings as the current settings
    tcsetattr(fileno(stdin), TCSANOW, &newSettings);
    // install the signal
    signal(SIGINT, quit);
    signal(SIGQUIT, quit);
    // while the user hasn't quit and
    // there is still time to count down from
    while(!e && time)
    {
        // wait for 1 second
        sleep(1);
        // display the time left
        printf("%d\n", --time);
        // spinlock
        while(p);
    }
    // return the settings back to normal
    tcsetattr(fileno(stdin), TCSANOW, &oldSettings);
    return;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        usage(argv[0]);
        return -1;
    }
    int time = atoi(argv[1]);
    if(time > 0)
        timer(time);
    else
        timer(time * -1);
    return 0;
}
