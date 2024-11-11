#include <stdio.h>

int main() {
    // File pointer for the file where the output will be redirected
    FILE *file;

    // Open a file for writing ("w" mode)
    file = freopen("output.txt", "w", stdout);

    // Check if the file was successfully opened
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Print some messages - these will be written to the file instead of the console
    printf("This message will be written to the file 'output.txt'.\n");
    printf("This demonstrates redirection of standard output in C.\n");
    printf("All subsequent printf statements will write to the file.\n");

    // Close the file when done
    fclose(file);

    return 0;
}
