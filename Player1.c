/*

Heloixcon
System Programming

Program 10: Tic-Tac-Toe


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
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>



#define FIFO_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define SHM_PERMS  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define MAX_BUF    1024

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
typedef struct {
int turn;
int board[3][3];
} shared_mem;

int main(int argc, char *argv[])
{
  int fd[2], my_turn, other_turn, shmid, i, j, r, init_done = 0, winner;
  int num1, num2;
  char buf[MAX_BUF];
  key_t key1, key2, key3;
  shared_mem *shm;
  sem_t *sem[2];

  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <X|O>\n", argv[0]);
    exit(1);
  }

  fd[0] = open("xoSync", O_RDONLY);
  fd[1] = open("xoSync", O_WRONLY);

  if (fd[0] == -1 && fd[1] == -1) 
  {
    fd[1] = open("xoSync", O_WRONLY | O_CREAT | O_EXCL, FIFO_PERMS);
    if (fd[1] != -1) 
    {
      srand(time(NULL));
      num1 = rand();
      num2 = rand();
      key1 = ftok("xoSync", num1);
      key2 = ftok("xoSync", num2);
      key3 = ftok("xoSync", num1+num2);
      shmid = shmget(key3, sizeof(shared_mem), IPC_CREAT | SHM_PERMS);
      shm = (shared_mem *)shmat(shmid, 0, 0);
      sem[0] = sem_open(key1, O_CREAT | O_EXCL, SHM_PERMS, 1);
      sem[1] = sem_open(key2, O_CREAT | O_EXCL, SHM_PERMS, 0);
      shm->turn = 0;
      for (i=0; i<3; i++)
      for (j=0; j<3; j++)
      shm->board[i][j] = 0;
      sprintf(buf, "%d %d", num1, num2);
      write(fd[1], buf, strlen(buf)+1);
      close(fd[1]);
    }
    else 
    {
      fd[0] = open("xoSync", O_RDONLY);
      fd[1] = open("xoSync", O_WRONLY);
      read(fd[0], buf, MAX_BUF);
      close(fd[0]);
      close(fd[1]);
      num1 = atoi(buf);
      num2 = atoi(buf+strlen(buf)+1);
      key1 = ftok("xoSync", num1);
      key2 = ftok("xoSync", num2);
      key3 = ftok("xoSync", num1+num2);
      shmid = shmget(key3, sizeof(shared_mem), SHM_PERMS);
      shm = (shared_mem *)shmat(shmid, 0, 0);
      sem[0] = sem_open(key1, 0);
      sem[1] = sem_open(key2, 0);
    }
  }
  else 
  {
    if (fd[0] != -1) 
    {
      my_turn = 1;
      other_turn = 0;
    }
    else 
    {
      my_turn = 0;
      other_turn = 1;
    }


    read(fd[0], buf, MAX_BUF);
    close(fd[0]);
    close(fd[1]);
    num1 = atoi(buf);
    num2 = atoi(buf+strlen(buf)+1);
    key1 = ftok("xoSync", num1);
    key2 = ftok("xoSync", num2);
    key3 = ftok("xoSync", num1+num2);
    shmid = shmget(key3, sizeof(shared_mem), SHM_PERMS);
    shm = (shared_mem *)shmat(shmid, 0, 0);
    sem[0] = sem_open(key1, 0);
    sem[1] = sem_open(key2, 0);
  }

  if (argv[1][0] == 'X') 
  {
    my_turn = 1;
    other_turn = 0;
  }
  else 
  {
    my_turn = 0;
    other_turn = 1;
  }

  while (1) 
  {
    sem_wait(sem[my_turn]);
    if (shm->turn == -1)
    break;
    for (i=0; i<3; i++) 
    {
      for (j=0; j<3; j++) 
      {
        if (shm->board[i][j] == 1)
        printf("X");
        else if (shm->board[i][j] == -1)
        printf("O");
        else
        printf(".");
     }
     printf("\n");
   }
   if (my_turn == 1) 
   {
     do 
     {
       printf("Enter row and column for X: ");
       scanf("%d %d", &i, &j);
     } while (i<0 || i>2 || j<0 || j>2 || shm->board[i][j] != 0);
       shm->board[i][j] = 1;
   }
   else 
   {
     if (shm->turn == 0) 
     {
       i = rand()%3;
       j = rand()%3;
       while (shm->board[i][j] != 0) 
       {
         i = rand()%3;
         j = rand()%3;
       }
       shm->board[i][j] = -1;
     }
   else 
   {
     winner = 0;
     for (i=0; i<3; i++) 
     {
       r = 0;
       for (j=0; j<3; j++)
       r += shm->board[i][j];
       if (r == -3) 
       {
         winner = -1;
         break;
       }
       else if (r == 3) 
       {
         winner = 1;
         break;
       }
    }
//

  if (winner == 0) 
  {
    for (j=0; j<3; j++) 
    {
      r = 0;
      for (i=0; i<3; i++)
      r += shm->board[i][j];
     if (r == -3) 
     {
       winner = -1;
       break;
     }
     else if (r == 3) 
     {
       winner = 1; 
       break;
     }
    }
  }
  if (winner == 0) 
  {
    r = 0;
    for (i=0; i<3; i++)
    r += shm->board[i][i];
    if (r == -3)
    winner = -1;
    else if (r == 3)
    winner = 1;
  }
  if (winner == 0) 
  {
    r = 0;
    for (i=0; i<3; i++)
    r += shm->board[i][2-i];
    if (r == -3)
    winner = -1;
    else if (r == 3)
    winner = 1;
  }
  if (winner == 0) 
  {
    for (i=0; i<3; i++) 
    {
      for (j=0; j<3; j++) 
      {
        if (shm->board[i][j] == 0) 
        {
          i = j = 3;
          break;
        }
      }
    }


    if (i == 3 && j == 3)
    shm->turn = -1;
  }
 }
 if (shm->turn != -1) 
 {
   for (i=0; i<3; i++) 
   {
     r = 0;
     for (j=0; j<3; j++)
     r += shm->board[i][j];
     if (r == -3) 
     {
       winner = -1;
       break;
     }
     else if (r == 3) 
     {
     winner = 1;
     break;
     }
   }
 if (winner == 0) 
 {
   for (j=0; j<3; j++) 
   {
     r = 0;
     for (i=0; i<3; i++)
     r += shm->board[i][j];
     if (r == -3) 
     {
       winner = -1;
       break;
     }
     else if (r == 3) 
     {
       winner = 1;
       break;
     }
   }
 }
 if (winner == 0) 
 {
   r = 0;
   for (i=0; i<3; i++)
   r += shm->board[i][i];
   if (r == -3)
   winner = -1;
   else if (r == 3)
   winner = 1;
 }
 if (winner == 0) 
 {
   r = 0;
   for (i=0; i<3; i++)
   r += shm->board[i][2-i];
   if (r == -3)
   winner = -1;
   else if (r == 3)
   winner = 1;
 }
 if (winner == 0) 
 {
   for (i=0; i<3; i++) 
   {
     for (j=0; j<3; j++) 
     {
       if (shm->board[i][j] == 0) 
       {
         i = j = 3;
         break;
       }
     }
   }
   if (i == 3 && j == 3)
   shm->turn = -1;
 }
   if (winner == 1)
   shm->turn = -1;
 }
    shm->turn++;
    sem_post(sem[other_turn]);
  }

  for (i=0; i<3; i++) 
  {
    for (j=0; j<3; j++) 
    {
      if (shm->board[i][j] == 1)
      printf("X");
      else if (shm->board[i][j] == -1)
      printf("O");
      else
      printf(".");
   }
   printf("\n");
 }

 if (shm->turn == -1) 
 {
   if (winner == 1)
   printf("X wins!\n");
   else if (winner == -1)
   printf("O wins!\n");
   else
   printf("Draw!\n");
 }
  sem_wait(sem[my_turn]);
  shm->turn = -1;
  sem_post(sem[other_turn]);
  return 0;
}
