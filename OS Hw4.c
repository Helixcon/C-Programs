#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {

 struct stat file_stats;
if (fstat(fd, &file_stats) < 0) {
  perror("fstat error");
  exit(1);
}

if (S_ISREG(file_stats.st_mode)) {
  printf("File type: regular file\n");
} else if (S_ISDIR(file_stats.st_mode)) {
  printf("File type: directory\n");
} else {
  printf("File type: unknown\n");
}
printf("Owner user ID: %d\n", file_stats.st_uid);
printf("Owner group ID: %d\n", file_stats.st_gid);
printf("Size: %ld bytes\n", file_stats.st_size);


  struct rusage usage;
if (getrusage(RUSAGE_SELF, &usage) < 0) {
  perror("getrusage error");
  exit(1);
}
printf("User CPU time used: %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
printf("System CPU time used: %ld.%06ld seconds\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);


  return 0;
}