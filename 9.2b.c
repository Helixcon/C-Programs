/*

Heloixcon


CSE 3183-02/ System Programming

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
  int fd = HandleError(open("test", O_RDONLY));
  char str[200];
  int n;
    
  HandleError(read(fd,&n,sizeof(int))<0);
   
  HandleError(read(fd, str,sizeof(char)*n)<0);
    
    
  printf("Child Received: %s\n",str);
  close(fd);
  
 
 exit(EXIT_SUCCESS);
}
