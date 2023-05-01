/*

Heloixcon/ System Programming

Program 9.1: sharing data between processes


*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<time.h>
#include<string.h>
#include<sys/wait.h>

//Function to Handle for Errors
int HandleError(int val)
{
  if(val == -1)
  {
    perror("Failure Dectected");
    exit(EXIT_FAILURE);
  }
  return val;
}



int main(int argc, char*argv[])
{
  int fd[2];
  
  
  
  if(pipe(fd)==-1)
  {
    return 1;
  }
  
  int pid = fork();
  if(pid < 0)
  {
    return 2;
  }
  
  if(pid == 0)
  {
   // child process
    close(fd[1]);
    char str[200];
    int n;
    
    HandleError(read(fd[0],&n,sizeof(int))<0);
   
    HandleError(read(fd[0], str,sizeof(char)*n)<0);
    
    
    printf("Child Received: %s\n",str);
    close(fd[0]);
    wait(NULL);
  }
  else
  {
  
  //parent process
    
    close(fd[0]);
    char str[200];
    printf("Parent write out string: ");
    fgets(str, 200, stdin);
    str[strlen(str) - 1] = '\0';
    
    int n = strlen(str)+1;
    HandleError(write(fd[1],&n,sizeof(int))<0);
    
    HandleError(write(fd[1], str,sizeof(char)* n)<0);
    
    close(fd[1]);
    
  }

 exit(EXIT_SUCCESS);
}
