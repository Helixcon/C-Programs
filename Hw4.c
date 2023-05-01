/*Heloixcon
System Programming

*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>
#include <time.h>



#define MAX_FILES 50

int main(int argc, char *argv[]) {
    struct stat sb;
    struct rusage usage;
    char* filename;
    int i, fd;

    if (argc > MAX_FILES + 1) {
        printf("Error: too many arguments. Maximum number of files is %d.\n", MAX_FILES);
        return 1;
    }

    for (i = 1; i < argc; i++) {
        filename = argv[i];
        fd = open(filename, O_RDONLY);
        if (fd == -1) {
            printf("Error opening file %s: %s\n", filename, strerror(errno));
            continue;
        }

        printf("File name: %s\n", filename);

        if (fstat(fd, &sb) == -1) {
            printf("Error getting file information for %s: %s\n", filename, strerror(errno));
            continue;
        }

        printf("File type:                ");
        switch (sb.st_mode & S_IFMT) {
            case S_IFREG:
                printf("Regular file\n");
                break;
            case S_IFDIR:
                printf("Directory\n");
                break;
            case S_IFLNK:
                printf("Symbolic link\n");
                break;
            default:
                printf("Unknown\n");
                break;
        }

        printf("Owner ID:                 %d\n", sb.st_uid);
        printf("Group ID:                 %d\n", sb.st_gid);
        printf("Number of links:          %ld\n", sb.st_nlink);
        printf("Last access time:         %s", ctime(&sb.st_atime));
        printf("Last modification time:   %s", ctime(&sb.st_mtime));
        printf("Last status change time:  %s", ctime(&sb.st_ctime));
        printf("File size in bytes:       %lld\n", (long long) sb.st_size);
        printf("Inode number:             %ld\n", sb.st_ino);
        close(fd);
    }

    if (getrusage(RUSAGE_SELF, &usage) == -1) {
        printf("Error getting resource usage information: %s\n", strerror(errno));
        return 1;
    }

    printf("User CPU time used:       %ld.%06ld s\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("System CPU time used:     %ld.%06ld s\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);

    return 0;
}

/*
gcc -o myfstat-netid myfstat-netid.c
./myfstat-netid file1.txt file2.txt file3.txt


*/
