/*

Alonzo Reed, ar2628

Operating Systems

Program 1b: Using execvp()

*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
  // creating variables for command and arguments
  char* command = "ls";
  char* arguments[50] = {"ls","-l","/usr",NULL};
  
  printf("Creating new process before using execvp\n");  
  
  if(fork()==0)
  {
    // The child process will be taken over by "ls -l"
    // So the execvp can run through everything correctly
    int status = execvp(command, arguments);
    
    printf("Now the child has been taking over by ls -l.\n");
    // if status is -1 then execvp was terminated normally
    if(status == -1)
    {
      printf("error");
      return 1;
    }
  }
  else
  {
    // Parent Process
    printf("The c program is back in the Parent\n");
  }

  exit(EXIT_SUCCESS);
}
