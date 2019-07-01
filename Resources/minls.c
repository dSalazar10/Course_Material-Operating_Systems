/*
 * Bare bones implementation of ls(1)
 * Based on section 1.4 pg 5
 */

#include "apue.c"
#include <dirent.h>
int log_to_stderr;
int main(int argc, char *argv[])
{
   DIR			*dp; 
   struct dirent	*dirp;

   if(argc != 2)
      err_quit("usage: ls directory_name");

   if((dp = opendir(argv[1])) == NULL)
      err_sys("can't open %s", argv[1]);
   while((dirp = readdir(dp)) != NULL)
      printf("%s\n", dirp->d_name);
   closedir(dp);
   exit(0);
}
