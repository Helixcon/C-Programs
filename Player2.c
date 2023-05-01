/*

Heloixcon System Programming

Program 10 Tic-Tac-Toe


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
  
 }
