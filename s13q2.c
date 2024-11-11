#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        // Ensure the user provided exactly one argument
        fprintf(stderr, "Usage: %s <prefix>\n", argv[0]);
        return 1;
    }

    const char *prefix = argv[1];
    DIR *dir = opendir(".");  // Open the current directory
    struct dirent *entry;

    if (dir == NULL) {
        perror("opendir failed");
        return 1;
    }

    int found = 0;  // Flag to check if any files are found

    // Read directory entries one by one
    while ((entry = readdir(dir)) != NULL) {
        // Check if the file name starts with the given prefix
        if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
            // If match found, print the file name
            printf("%s\n", entry->d_name);
            found = 1;
        }
    }

    if (!found) {
        printf("No files found that begin with '%s'\n", prefix);
    }

    closedir(dir);  // Close the directory
    return 0;
}
