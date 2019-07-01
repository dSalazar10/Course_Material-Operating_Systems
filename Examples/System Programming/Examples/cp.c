#include <stdlib.h>
#include <stdio.h>

int main( int ac, char* args[] )
{
  FILE* sp, *dp;
  
  if( ac != 3 )
  {
    printf("Usage: cp SRC DEST\n");
    exit(1);
  }
  
  sp = fopen( args[1], "r" );
  
  if( !sp )
  {
    printf("Error opening source file.\n");
    exit(2);
  }
  
  dp = fopen( args[2], "w" );
  
  if( !dp )
  {
    printf("Error opening destination file.\n");
    exit(3);
  }
  
  char* in = (char*)malloc(80);
  
  // see stdio.h for more info on fgets and fputs
  while( ( fgets( in, 80, sp) ) != NULL )
    fputs(in, dp);
    
  close(sp);
  close(dp);
  free(in);
  
  return 0;
}