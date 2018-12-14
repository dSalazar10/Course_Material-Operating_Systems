#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char argv[])
  {
  char  line[100];
  char *args[100];
  int   num_args;
  int   i;
  int   waitpid;
  int   childpid;
  int   status;

  while (1)
    {
    printf("Enter command: ");
    gets(line);
    if(strcmp(line, "quit") == 0)
      break;
    num_args = 0;
    args[num_args] = strtok(line, " ");
    while (args[num_args] != NULL)
      {
      num_args++;
      args[num_args] = strtok(NULL, " ");
      }
    num_args--;
    printf("About to exec the following: ");
    for (i = 0; i <= num_args; i++)
      {
      printf(args[i]);
      printf(" ");
      }
    printf("\n");
    printf("\n");
    childpid = fork();
    if (childpid == 0)
      {
      execvp(args[0], args);
      perror("Exec failed: ");
      exit(0);
      }
    waitpid = wait(&status);
    printf("Shell process %d exited with status %d\n", waitpid, (status >> 8));
    }
  return 0;
 }
