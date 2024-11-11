#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main() {
    DIR *dir;
    struct dirent *entry;
    int file_count = 0;

    // Open the current directory (".") for reading
    dir = opendir(".");

    if (dir == NULL) {
        perror("opendir");  // If opendir fails
        return 1;
    }

    // Print the names of files and directories
    printf("Files in the current directory:\n");

    // Read each entry in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Skip the special entries "." and ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s\n", entry->d_name);
            file_count++;
        }
    }

    // Close the directory
    closedir(dir);

    // Print the number of files (excluding "." and "..")
    printf("\nTotal number of files in the current directory: %d\n", file_count);

    return 0;
}
