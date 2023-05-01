/*Heloixcon
System Programming

*/



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_FILENAME_LEN 256
#define MAX_THREADS 100

// Global variables
int total_lines = 0;
pthread_mutex_t total_mutex;

// Function prototypes
void* count_lines(void* arg);
void update_total(int subtotal);

int main(int argc, char** argv) {
    // Check command line arguments
    if (argc < 2) {
        printf("Usage: %s file1 [file2 ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // Initialize mutex
    if (pthread_mutex_init(&total_mutex, NULL) != 0) {
        printf("Error: failed to initialize mutex.\n");
        exit(EXIT_FAILURE);
    }
    
    // Create threads
    int num_threads = argc - 1;
    pthread_t threads[MAX_THREADS];
    for (int i = 0; i < num_threads; i++) {
        char* filename = argv[i+1];
        pthread_create(&threads[i], NULL, count_lines, filename);
    }
    
    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Destroy mutex
    pthread_mutex_destroy(&total_mutex);
    
    // Print total lines
    printf("Total lines: %d\n", total_lines);
    
    return 0;
}

void* count_lines(void* arg) {
    // Get filename argument
    char* filename = (char*) arg;
    
    // Open file
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: failed to open file '%s'.\n", filename);
        return NULL;
    }
    
    // Count lines
    int subtotal = 0;
    char line[MAX_FILENAME_LEN];
    while (fgets(line, MAX_FILENAME_LEN, fp) != NULL) {
        subtotal++;
    }
    
    // Print subtotal
    printf("%s: %d lines\n", filename, subtotal);
    
    // Update total
    update_total(subtotal);
    
    // Cleanup
    fclose(fp);
    return NULL;
}

void update_total(int subtotal) {
    // Lock mutex
    pthread_mutex_lock(&total_mutex);
    
    // Update total
    total_lines += subtotal;
    
    // Unlock mutex
    pthread_mutex_unlock(&total_mutex);
}


