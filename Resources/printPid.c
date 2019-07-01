#include "apue.c"
int log_to_stderr;

int main(void)
{
   printf("Hello world from proces ID %ld\n", (long)getpid());
   exit(0);
}
