#include <stdio.h>
#include <dirent.h>
#include <string.h>

int file_exists_in_directory(const char *filename) {
    struct dirent *entry;
    DIR *dp = opendir(".");  // Open the current directory

    if (dp == NULL) {
        perror("opendir");
        return 0;  // Return 0 if directory cannot be opened
    }

    // Read each entry in the current directory
    while ((entry = readdir(dp)) != NULL) {
        // Compare the current entry's name with the filename
        if (strcmp(entry->d_name, filename) == 0) {
            closedir(dp);  // Close the directory after use
            return 1;  // Return 1 if the file is found
        }
    }

    closedir(dp);  // Close the directory after use
    return 0;  // Return 0 if the file is not found
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;  // Return if no filename is provided
    }

    const char *filename = argv[1];

    if (file_exists_in_directory(filename)) {
        printf("File '%s' exists in the current directory.\n", filename);
    } else {
        printf("File '%s' does not exist in the current directory.\n", filename);
    }

    return 0;
}
