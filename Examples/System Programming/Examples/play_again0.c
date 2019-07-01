/*  play_again1.c
	purpose:	ask if user wants another transaction
	method:		read char, return result
	returns:	0=>yes, 1=>no
	better:		set tty into char-by-char mode to not echo inappropriate input
*/

#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response( char* );

int main( int ac, char* av[] )
{
	int response;
	
	tty_mode(0);				// save tty mode
	set_crmode();				// set chr-by-chr mode
	response = get_response( QUESTION );  // get some answer
	tty_mode(1);				// restore tty mode
	
	return response;
}

int get_response( char* question )
/* 
	purpose:	ask a question and wait for a y/n answer
	method:		use getchar and complain about non y/n answers
	returns:	0=>yes, 1=>no
*/
{
	int input;
	
	printf("%s (y/n)?", question);
	while(1) 
	{
		switch( input = getchar() )
		{
			case 'y':
			case 'Y':
				return 0;
			case 'n':
			case 'N':
			case EOF:
				return 1;
			default:
				printf("\ncannot understand %c, ", input );
				printf("Please type y or n.\n: ");
		}
	}
}

