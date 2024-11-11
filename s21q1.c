#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main() {
    DIR *dir;
    struct dirent *entry;
    int file_count = 0;

    // Open the current directory (".")
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    printf("Files in the current directory:\n");

    // Read all entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Exclude "." and ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s\n", entry->d_name);  // Display the file name
            file_count++;                   // Increment the file count
        }
    }

    // Close the directory
    closedir(dir);

    printf("\nTotal number of files: %d\n", file_count);

    return 0;
}
