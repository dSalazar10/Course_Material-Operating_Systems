//
//  id.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/7/18.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    
    int val;
    
    printf("This process is %d\n", getpid());
    printf("The real user ID is %d\n", getuid());
    printf("The effective user ID is %d\n", geteuid());
    
    if((val=fork())<0)
    {
        printf("Problem creating new process\n");
        return 1;
    }
    if(val==0)
    {
        printf("This process is %d\n", getpid());
        printf("The real user ID is %d\n", getuid());
        printf("The effective user ID is %d\n", geteuid());
    }
    return 0;
}
