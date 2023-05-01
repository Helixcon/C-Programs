/*

Heloixcon/ System Programming

Program 9.2: sharing data through FIFOs


*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>

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


int main(int argc, char* argv[])
{
  if(mkfifo("test", S_IRWXU) == -1)
  {
    if(errno != EEXIST)
    {
      perror("mkfifo producer");
      exit(EXIT_FAILURE);
    }
  }
  
  int fd = HandleError(open("test",O_WRONLY));
  
  
  char str[200];
  printf("Parent write out string: ");
  fgets(str, 200, stdin);
  str[strlen(str) - 1] = '\0';
    
  int n = strlen(str)+1;
  HandleError(write(fd,&n,sizeof(int))<0);
    
  HandleError(write(fd, str,sizeof(char)* n)<0);
    
  close(fd);
   

  exit(EXIT_SUCCESS);
}
