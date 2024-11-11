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
        perror("Unable to open the current directory");
        return 1;
    }

    // Read the directory entries
    printf("Files in the current directory:\n");
    while ((entry = readdir(dir)) != NULL) {
        // Skip the special entries "." and ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s\n", entry->d_name);  // Display file name
            file_count++;  // Increment the file count
        }
    }

    // Display the total number of files
    printf("\nTotal number of files: %d\n", file_count);

    // Close the directory
    closedir(dir);

    return 0;
}
