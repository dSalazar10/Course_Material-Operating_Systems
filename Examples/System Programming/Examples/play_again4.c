/* play_again4.c
    purpose:    ask if user wants another transaction
    method:     set tty into chr-by-chr, no-echo mode
                set tty into no-delay mode
                read char, return result
                resets terminal modes on SIGINT, ignores SIGQUIT
    returns:    0=>yes, 1=>no, 2=>timeout
    better:     reset terminal mode on Interrupt
*/

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define ASK "Do you want another transaction"
#define TRIES 3                 // max tries
#define SLEEPTIME 2             // time per try
#define BEEP putchar('\a')      // alert user

void ctrl_c_handler( int );
void tty_mode( int );
void set_cr_noecho_mode();
void set_nodelay_mode();
int get_response( char*, int );
int get_ok_char();

int main()
{
    int response;
    
    tty_mode(0);                                // save current mode
    set_cr_noecho_mode();                       // set -icanon -echo
    set_nodelay_mode();                         // noinput ==> EOF
    signal ( SIGINT, ctrl_c_handler );          // handle INT
    signal ( SIGQUIT, SIG_IGN );                // ignore QUIT signals
    response = get_response( ASK, TRIES );      // get some answer
    tty_mode(1);                                // reset original mode
   
    return response;
}

// ask a question and wait for y/n answer or timeout
// uses getchar and complains about non-y/n input
// returns 0=>yes, 1=>no
int get_response( char* q, int maxtries )
{
    int input;
    
    printf( "%s (y/n)?", q );                   // ask
    fflush( stdout );                           // force output
    while( 1 )
    {
        sleep( SLEEPTIME );                     // wait
        input = tolower( get_ok_char() );       // get next char
        if ( input == 'y' )
            return 0;
        if ( input == 'n' )
            return 1;
        if ( maxtries-- == 0 )                  // out of tries?
            return 2;
            
        BEEP;
    }
}

// skip over non-legal chars and return y, Y, n, N, or EOF
int get_ok_char()
{
    // getchar reads the next char in the stream and returns it as
    // an unsigned char cast as an int.  So, it's returning the
    // ascii code for the character it read.
    int c;
    while( ( c = getchar() ) != EOF && strchr( "yYnN", c ) == NULL ) ;
    
    return c;
}

// put file descriptor 0 into chr-by-chr  mode and no echo mode
// uses bits in termios
void set_cr_noecho_mode()
{
    struct termios ttystate;
    
    tcgetattr( 0, &ttystate );                  // read curr settig
    ttystate.c_lflag &= ~ICANON;                // no buffering
    ttystate.c_lflag &= ~ECHO;                  // no echo either
    ttystate.c_cc[VMIN] = 1;                    // get 1 char at a time
    tcsetattr( 0, TCSANOW, &ttystate );         // install settings
}

// put file descriptor 0 into no-delay mode
// use fcntl to set bits
void set_nodelay_mode()
{
    int termflags;
    
    termflags = fcntl( 0, F_GETFL );            // read curr settings
    termflags |= O_NDELAY;                      // flip no delay bit
    fcntl( 0, F_SETFL, termflags );             // install 'em
}

// how == 0=> save current mode,    how == 1 => restore mode
// handles termios and fcntl flags
void tty_mode( int how )
{
    static struct termios original_mode;
    static int original_flags;
    static int stored = 0;
    
    if( how == 0 )
    {
        tcgetattr( 0, &original_mode );
        original_flags = fcntl( 0, F_GETFL );
        stored = 1;
    }
    else if ( stored )
    {
        tcsetattr( 0, TCSANOW, &original_mode );
        fcntl( 0, F_SETFL, original_flags );
    }
}

// called if SIGINT is detected - resets tty and exits
void ctrl_c_handler( int signum )
{
    tty_mode(1);
    exit(1);
}

