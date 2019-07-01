//
//  timer.c
//  programs
//
//  Created by DANIEL SALAZAR on 5/8/18.
//
// "Description
// Displays a count down to zero from a number supplied by the user at the command line.  Supports
// pausing and unpausing the countdown.  Countdown is done by ones, one second at a time.  While
// the program is runnng, typing p will pause the countdown and typing u will unpause it.  echoing of
// characters is disabled while the program is running."

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

void usage(char name[])
{
    printf("Usage: %s [number]\n", name);
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
    // pause output
    newSettings.c_cc[VSTOP] = 'p';
    // unpause output
    newSettings.c_cc[VSTART] = 'u';
    // Set new settings as the current settings
    tcsetattr(fileno(stdin), TCSANOW, &newSettings);
    // while there is still time to count down from
    while(time)
    {
        // wait for 1 second
        sleep(1);
        // display the time left
        printf("%d\n", --time);
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
