#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    // Accept the threshold file size in bytes from the user
    long n;
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
    int found = 0;  // Flag to track if any matching files are found

    // Read the directory entries one by one
    while ((entry = readdir(dir)) != NULL) {
        // Ignore the special entries "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Get the file status using stat()
        if (stat(entry->d_name, &fileStat) == -1) {
            perror("stat failed");
            continue;
        }

        // If the file size is greater than the specified threshold, print its name
        if (fileStat.st_size > n) {
            printf("File: %s, Size: %ld bytes\n", entry->d_name, fileStat.st_size);
            found = 1;
        }
    }

    // If no file found with size greater than n bytes
    if (!found) {
        printf("No files found larger than %ld bytes.\n", n);
    }

    closedir(dir);  // Close the directory
    return 0;
}
