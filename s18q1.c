#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Check if the user has provided the file name as an argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    DIR *dir;
    struct dirent *entry;
    int found = 0;

    // Open the current directory
    dir = opendir(".");
    if (dir == NULL) {
        perror("Unable to open the current directory");
        return 1;
    }

    // Read the directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Compare each entry's name with the provided filename
        if (strcmp(entry->d_name, filename) == 0) {
            found = 1;  // File is found
            break;
        }
    }

    // Close the directory
    closedir(dir);

    // Output the result
    if (found) {
        printf("File '%s' is present in the current directory.\n", filename);
    } else {
        printf("File '%s' is not present in the current directory.\n", filename);
    }

    return 0;
}
