#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<fcntl.h>

#define oops(s,x)  { perror(s); exit(x); }
#define	BUFLEN		128
#define	FIFOMODE 	0644

int main(int ac, char *av[])
{
	int	fd;		            /* the fifo 	*/
	char	buf[BUFLEN];	/* the string	*/
	int	n;

	if ( ac != 2 )
    {
		fprintf(stderr,"usage: %s fifoname\n", *av);
		exit(1);
	}


	if ( (fd = open(av[1], O_RDONLY)) == -1 )
		oops(av[1],2);

	while( (n = read(fd, buf, BUFLEN)) > 0 )
		if ( write(1, buf, n) == -1 )
			oops("write",3);

	close(fd);

	return 0;
}
