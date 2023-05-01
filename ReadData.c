/*

Heloixcon/ System Programming

Program 7:ReadChildData

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

/*struct for char & int
  struct for nanosleep

open data file

for loop to read file line by line

nanosleep in btw to tell how long to wait to execute

while loop to print out data

*/

#define BUF_SIZE 1024

struct pair
{
  char val;
  short int cnt;
};

int HandleError(int val, const char*str)
{
  if(val == -1)
  {
    perror(str);
    exit(EXIT_FAILURE);
  }
  return val;
}

int myRand(int low, int high)
{
  int rng = high - low + 1;
  static int init;
  double mult;

  
  if (!init) { srand(time(NULL)); init = 1;}

  mult = rand() / (double) RAND_MAX;

  return low + (rng * mult);
}

void CreateData()
{
  int i = 0;
  int cnt = 100;
  char fname[1024] = "Ximage.dat";
  int fd;
  int numWrit;
  int argc;
  char *argv[1024];
  struct pair *arr;

  if (argc > 2)
    {
      strcpy(fname, argv[1]);
      sscanf(argv[2],"%d",&cnt);
    }
      

  
  arr = (struct pair *) calloc(cnt, sizeof (struct pair));
  if (arr == NULL) HandleError(-1,"calloc");

  
  
  for (i = 0; i < cnt; i++)
    {
      arr[i].val = myRand(0,255);
      arr[i].cnt = myRand(1,100);
    }


  fd = HandleError(open(fname,O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR), "open");

  numWrit = HandleError(write(fd, arr, cnt * sizeof(struct pair)), "write");
  
  close(fd);

}

int main(int argc, char *argv[])
{
  // struct for nanosleep
  struct timespec ts;
  ts.tv_sec = 1;
  ts.tv_nsec = 0;
  // declare an instance of struct pair ...
  struct pair arr; 
  int fd;
  int count = 60;
  int i =0;
  

  pid_t childPID;
  
  
  
  printf("Inside the parent before creating child\n");
  printf("Calling fork now\n");
  
  switch(childPID = fork())
  {
  case -1:
    HandleError(-1,"fork");
  case 0:/*child to use myRand program*/
    CreateData();
    printf("Generating data file completed. Now back to parent for reading.\n");
    
  default:/*parent*/
    
    
    char buffer[BUF_SIZE];
    fd = HandleError(open("Ximage.dat",O_RDONLY),"open"); 
  
    // loop to read file 
    // argument to read are the fd, a buffer, and a size .. 
    // if the buffer isn't a pointer, the don't forget the & operator
    while(read(fd,&arr,sizeof(struct pair)) > 0)
    {
      // loop to print out file content
    
   	  printf("%c %d\n",arr.val, arr.cnt);
   	
    
      // as for the loop .. the count is the cnt field of the currently read struct pair
      // the charcter is the val field of the currently read struct pair
    
     for(i= 0; i< arr.cnt;i++)
     {
      // printf(format string, vars) it is never printf(format string, var, format, var)
      // printf("%c %d", chr, int) not printf("%c", char, "%d", int)
        printf("%c",arr.val);
  
     }
      //nanosleep
      printf("\n");
      nanosleep(&ts,NULL);
  
  }

  close(fd);
    }
  
  exit(EXIT_SUCCESS);


}


