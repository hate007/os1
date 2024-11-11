#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

int main() {
    int month, year;

    // Accept month and year from the user
    printf("Enter the month (1-12): ");
    if (scanf("%d", &month) != 1 || month < 1 || month > 12) {
        fprintf(stderr, "Invalid month input.\n");
        return 1;
    }

    printf("Enter the year (e.g., 2024): ");
    if (scanf("%d", &year) != 1) {
        fprintf(stderr, "Invalid year input.\n");
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
    struct tm *timeinfo;
    int found = 0;  // Flag to track if any files are found

    // Read the directory entries
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

        // Convert modification time to struct tm
        timeinfo = localtime(&fileStat.st_mtime);
        if (timeinfo == NULL) {
            perror("localtime failed");
            continue;
        }

        // Check if the file's modification month and year match the user input
        if (timeinfo->tm_mon + 1 == month && (timeinfo->tm_year + 1900) == year) {
            printf("File: %s, Last modified: %s", entry->d_name, asctime(timeinfo));
            found = 1;
        }
    }

    // If no files are found, inform the user
    if (!found) {
        printf("No files found for %d-%02d.\n", year, month);
    }

    // Close the directory
    closedir(dir);

    return 0;
}
