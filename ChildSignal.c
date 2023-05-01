/*

Heloixcon

CSE 3183-02/ System Programming

Program 6: Handling multiple signal from child

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
/*
declare variables
Parent exit function
child exit function
*/

int cno = 0;
int stage = 0;
pid_t pid[3];
char reply;

void exitP(void)
{
  char *out = "Parent has normally terminated\n";
  write(STDOUT_FILENO, out, strlen(out));
}

void exitC(void)
{
  char *out = "Child has normally terminated\n";
  write(STDOUT_FILENO, out, strlen(out));
}

//Parent Signhandler(SIGCHILD,SIGINT,SIGUSR1,SIGUSR2)

void signalHandlerP(int sig)
{
  char other[1024] = { 0 };
  pid_t p;
  int status,i; 

  if(sig == SIGCHLD)
  {
    if (p == -1)
	  {
	    if (errno == ECHILD)
	      {
	        exit(EXIT_SUCCESS);
	      }
	    perror("waitpid");
	    exit(EXIT_FAILURE);
	  }
    printf("All the child have been ");
    while(waitpid(-1,NULL, WNOHANG)>0) continue;
    exit(EXIT_SUCCESS);
  }
  if(sig == SIGINT)
  {
    printf("Exit: Are you sure (y/n)?");
    // read the reply before testing it
    scanf("%c", &reply);
    result = strcmp("y",reply);
    
 
    // and must use strcmp or strncmp when testing strings like "Y"
    // otherwise would need to do something like replay[0] == 'Y' 
    // can't just do replay == "Y" .. doesn't work in C.
    if(result == 0 )
    {
    // send SIGTERM to the child
    // recall this uses something like
    // kill(pid, SIGTERM);
      kill(pid[i],SIGTERM);
      exit(EXIT_SUCCESS);
    }
    if(result < 0)
    {
      return;
    }
  }
  if(sig == SIGUSR1)
  {
  // replace X with %d and make certain you actually have a variable you are incrementing ...
    printf("The child has generated %d\n value less than 25.", &rand_num);
    //pause();
  }
  if(sig == SIGUSR2)
  {
  // same here
    printf("The child has generated %d\n value greater than 75.", &rand_num);
    //pause();
  }
  // move this block of code into the SIGCHLD handler
  // from here
	// to here

}

//Child Signhandler(SIGINT,SIGTERM)
// if using an interval timer, the child handler also needs to handle
// SIGALRM .. 
void signalHandlerC(int sig)
{
// best to remove SIGINT .. we should really use a proc mask to prevent the child 
// from ever handling it
  
  if(sig == SIGINT)
  {
   sigset_t mask;
   sigemptyset(&mask);
   sigaddset(&mask SIGINT);
   sigprocmask(SIG_BLOCK, &mask,null);
  }
  
  if(sig == SIGTERM)
  {
    // simply exit the child with a status of EXIT_SUCCESS
    
    exitC();
    exit(EXIT_SUCCESS);
  }


}

int main(int argc, char *argv[])
{

  int i = 0; /* loop counter */

  struct sigaction saP;
  struct sigaction saC;
  struct itimerval tv;


  saP.sa_handler = signalHandlerP;
  sigemptyset(&saP.sa_mask);
  saP.sa_flags = 0;

  saC.sa_handler = signalHandlerC;
  sigemptyset(&saC.sa_mask);
  saC.sa_flags = 0;
  
  tv.it_interval.tv_sec = 15;
  tv.it_interval.tv_usec = 0;
  tv.it_value.tv_sec = 15;
  tv.it_value.tv_usec = 0;
  
  /* first, add the signal hanlder for SIGCHLD */

  if (sigaction(SIGCHLD, &saP, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
    if (sigaction(SIGINT, &saC, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  

  

  // do standard testing to determine if in child or parent
  // if fork == 0
  // add signal handlers for SIGALRM and SIGTERM
   if (sigaction(SIGALRM, &saC, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
    if (sigaction(SIGTERM, &saC, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
    
  /* fork the child(s) */
  
  
  for (i = 0; i < 3; i++)
    {
      pid[i] = fork();
      switch(pid[i])
	{
	case -1:
	  perror("fork");
	  exit(EXIT_FAILURE);
	case 0:
	  atexit(exitC);
	  if (sigaction(SIGHUP, &saC, NULL) == -1)
	    {
	      perror("sigaction");
	      exit(EXIT_FAILURE);
	    }
	  printf("Starting child %d\n",i+1);
	  while (1)
	    {
	      pause();
	    }
	  exit(EXIT_SUCCESS);
	  break;
	}
    }
    
  // set the proc mask for SIGINT
  // recall that the proc mask requires
  // a variable of type sigset_t
  // as well as sigempty, sidaddset, and sigprocmask 
  // you can look these up in the man pages or the  notes
  // now start your interval timer .. that's a setitimer if you have forgotten
  // then loop forever
  // in that loop
  //    generate a random number between 0 and 100
  //    if that number is < 25, send SIGUSR1 to parent .. that is using kill .. so something like kill(parent, SIGUSR1)
  //    if that number is > 75, send SIGUSR2 to parent .. see above
  //    if that number if > 48 and < 51, then exit the child with a value of EXIT_SUCCESS
  //    now if you are using an interval time, simply pause()
  //    otherwise use sleep or nanosleep

  for(i=0;i<3; i++)
  {
    
  
  
  }
// if the fork was > 0, we are in the parent
// add signal handlers for SIGINT, SIGUSR1, SIGUSR2
// recall that is sigaction(SIGINT, &sa, NULL) .. etc
// then simply enter a busy pause loop
// which is
// while(1) pause();  
  
  
  
  
  
  /* back in the parent */
  exit(exitP);
  if (sigaction(SIGALRM, &saP, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  if (setitimer(ITIMER_REAL, &tv, NULL) == -1)
    {
      perror("setitimer");
      exit(EXIT_FAILURE);
    }
  while(1)
    {
      pause();
    }
  exit(EXIT_SUCCESS);
  
}

