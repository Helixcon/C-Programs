/*

Heloixcon



Operating Systems

Program 1: Parent and Child counters

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
  int i = 0;// variable for the loop
  
  // Parent process that will run a counter 0-19
  if(fork() != 0)
  {
    printf("This is the Parent Process.\n");
    
    for(i = 0; i <= 19;i++)
    {
      printf("%d\n",i);
    }
    // Wait for the child process to finish
    wait(NULL);
  }
  // child process that will run a counter 0-19
  else
  {
    printf("This is the Child process\n");
    
    for(i = 0; i <= 19;i++)
    {
      printf("%d\n",i);
    }
    
  }
  
  exit(EXIT_SUCCESS);
  
}



