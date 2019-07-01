#include "apue.c"
int log_to_stderr;
int main()
{
   printf("uid = %d, gid = %d\n", getuid(), getgid());
   exit(0);
}
