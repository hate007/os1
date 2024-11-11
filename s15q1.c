#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int main() {
    long n;
    
    // Accept the size threshold from the user
    printf("Enter the size threshold (in bytes): ");
    if (scanf("%ld", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    // Open the current directory
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir failed");
        return 1;
    }

    struct dirent *entry;
    struct stat fileStat;
    int found = 0;  // Flag to track if any files are found

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip the special entries "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Get the file status
        if (stat(entry->d_name, &fileStat) == -1) {
            perror("stat failed");
            continue;
        }

        // Check if file size is greater than the threshold
        if (fileStat.st_size > n) {
            printf("File: %s, Size: %ld bytes\n", entry->d_name, fileStat.st_size);
            found = 1;
        }
    }

    // If no file matches the criteria, inform the user
    if (!found) {
        printf("No files found larger than %ld bytes.\n", n);
    }

    // Close the directory
    closedir(dir);
    
    return 0;
}
