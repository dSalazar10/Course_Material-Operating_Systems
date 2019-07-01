#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256

void error(const char*, int);				// error-handling function

int main(int ac, char* args[])
{
	char buffer[SIZE];						// buffer
	int lines, 								// lines to print
		bytes_read, 						// how many bytes read 
		fd, 								// file descriptor	
		i, j, 								// counters
		arg_num;							// helps with open						
	
	if(ac == 2)
	{
		lines = 10;
		arg_num = 1;
	}
	else if(ac == 3)
	{
		lines = atoi(args[1] + 2);			// love me some pointer arithmetic	
		arg_num = 2;		
	}
	else
	{
		printf("USAGE: %s [OPTION] [FILE]\n", args[0]);
		exit(0);
	}
	
	fd = open(args[arg_num], O_RDONLY);		// attempt to open file
	
	if(fd == -1)							// failure?  quit.
		error("Couldn't open file.", -1);
	
	j = 0;									// j counts \n characters
	
	// read into the buffer 
	while((bytes_read = read(fd, buffer, SIZE)) > 0)
	{
		i = 0;
		
		// scan the buffer, printing out characters and counting 
		// new lines as we go.  Only scan the part of the buffer 
		// containing characters we actually read in.
		while(i < bytes_read && j < lines)
		{
			printf("%c", buffer[i]);
			if(buffer[i] == '\n')
				j++;
			
			i++;
		}
	}
	
	// we should show an error message if read returns -1.
	if(bytes_read == -1)
		error("Error reading file.", -1);
	
	// same
	if(close(fd) == -1)
		error("Error closing file.", -1);
}

void error(const char* msg, int code)
{
	perror(msg);
	exit(code);
}
