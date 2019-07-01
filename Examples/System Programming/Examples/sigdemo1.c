/*
	sigdemo1.c - shows how a signal handler works.
				- run this and press Ctrl-C a few times
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void f( int ); // the handler

int main( int ac, char* av[] )
{
	int i;
	
	signal( SIGINT, f ); // install the handler
	
	for( i = 0; i < 5; i++ )  // do something
	{
		printf("hello\n");
		sleep(1);
	}
	
	return 0;
}

void f( int signum )  // the handler
{
	printf("OUCH!\n");
}