#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>


int main( int ac, char* args[] )
{
  if( ac != 2 )
  {
    printf("Usage: %s INODE\n", args[0] );
    exit( 1 );
  }

  // look up inode number
  int inode = atoi( args[1] ); 

  DIR* dp = NULL;
  if( ( dp = opendir( "." ) ) == NULL )
  {
    printf( "Failed inode lookup\n" );
    exit( 2 );
  }

  struct dirent* buffer;
  struct stat attribs;

  while( buffer = readdir( dp ) )
    if( buffer->d_ino == inode )
      break; 

  // attempt to remove directory entry if the entry was found
  // if it was, we need to check what type of entry it was.
  if( buffer && ( stat( buffer->d_name, &attribs ) == 0  ) )
  {
    // if it's a regular file, use unlink.
    if(  S_ISREG( attribs.st_mode ) && ( unlink( buffer->d_name )  == 0 ) )
    {
      closedir( dp );
      return 0;
    }       
    // otherwise, we have to use rmdir
    else if ( S_ISDIR( attribs.st_mode ) && rmdir( buffer->d_name ) == 0 )
    {
      closedir( dp );
      return 0;
    }  
  }
  
  // if we get here, everything was a failure. 
  printf( "Failed to remove directory entry.\n" ); 
}
