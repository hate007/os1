#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    // Check if at least one file name is passed
    if (argc < 2) {
        printf("Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return 1;
    }

    // Loop through each file passed as command-line argument
    for (int i = 1; i < argc; i++) {
        struct stat buffer;   // To store file status
        int result = stat(argv[i], &buffer);  // Check the status of the file

        // If stat returns 0, the file exists
        if (result == 0) {
            // Check if it's a regular file
            if (S_ISREG(buffer.st_mode)) {
                printf("File '%s' exists in the current directory.\n", argv[i]);
            } else {
                printf("'%s' exists but is not a regular file.\n", argv[i]);
            }
        } else {
            // If stat returns -1, the file does not exist or an error occurred
            perror("Error");
            printf("File '%s' does not exist in the current directory.\n", argv[i]);
        }
    }

    return 0;
}
