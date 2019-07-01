/*
    Write a timer program that accepts a number of seconds to count down
    from as an argument.
    
    The program counts down from that number until 0, decerementing every
    second.
    
    It displays the time remaining.
    
    It reprograms the terminal driver to support pausing the timer.  It does
    this by reprogramming terminal settings VSTART and VSTOP to S and Q.
    S - starts the timer / Q - pauses the timer.
    
    When the user types Q or S, that is not shown on the screen, so the 
    program should disable echoing while it's running as well.    
    ---           
    The program supports the following commands :
    p - pauses the timer
    u - unpauses the timer
*/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(int ac, char* args[])
{    
    struct termios old_settings, new_settings;
    
    // get the terminal settings
    tcgetattr(0, &old_settings);
    tcgetattr(0, &new_settings);
    
    // update them
    
    // turn off echoing
    new_settings.c_lflag &= ~ECHO;
    
    // change VSTART to 'u'
    new_settings.c_cc[VSTART] = 'u'; 
    
    // change VSTOP to 'p'
    new_settings.c_cc[VSTOP] = 'p';
        
    // send them back
    tcsetattr(0, TCSANOW, &new_settings);
       
    // do the countdown thing
    int seconds = atoi(args[1]);
   
    
    while(seconds >= 0)
    {                
        printf("%i\n", seconds);
        sleep(1);        
        seconds--;        
    }

    // restore original settings
    tcsetattr(0, TCSANOW, &old_settings);
}
