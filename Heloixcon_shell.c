#include <stdio.h>
#include <unistd.h>
#include<signal.h>
#define HISTORY_SIZE 10

/*

Heloixcon

Operating Systems

Program 2

*/
int main() {
   
   int choice;
   
    do {
        printf("\n\nMenu Options\n");
        printf("1. Option cd\n");
        printf("2. Option History\n");
        printf("3. Option Envir\n");
        printf("4. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("You chose Option cd\n");
                // CD function

   		    char path[100];
                printf("Enter path to change directory: ");
                scanf("%s", path);

                if (chdir(path) != 0) {
                printf("Failed to change directory\n");
                } else {
                printf("Directory changed successfully\n");
                }

                break;
            case 2:
                printf("You chose Option History\n");
                
                // History function

    			FILE *fp;
                  char *line = NULL;
                  size_t len = 0;
                  ssize_t read;
                  int i, count = 0;
                  char *history[HISTORY_SIZE];

                  fp = fopen(".bash_history", "r");
                  if (fp == NULL) {
                  printf("Failed to open history file\n");
                  exit(EXIT_FAILURE);
                  }

    	          while ((read = getline(&line, &len, fp)) != -1) {
          	  if (count < HISTORY_SIZE) {
            	  history[count] = line;
            	  count++;
        		} else {
                    free(history[0]);
            	  for (i = 1; i < HISTORY_SIZE; i++) {
                	  history[i - 1] = history[i];
                  }
                  history[HISTORY_SIZE - 1] = line;
                 }
                 line = NULL;
                 }

                 printf("Last %d commands:\n", count);
                 for (i = 0; i < count; i++) {
                 printf("%d: %s", i + 1, history[i]);
               } 

               fclose(fp);
               if (line) {
               free(line);
               }
                break;
            case 3:
                printf("You chose Option Envir\n");
                // envir function

                char *path = getenv("PATH");
                if (path) {
                printf("PATH=%s\n", path);
                } else {
                printf("PATH environment variable not found\n");
                }

                break;
            case 4:
                printf("Exiting program...\n");
                exit(EXIT_SUCCESS);
                break;
            default:
                printf("Invalid choice, please try again\n");
                break;
        }
    } while (choice != 4);


 }


   return 0;
}


