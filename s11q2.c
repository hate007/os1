#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    // Open the file "output.txt" for writing (create it if it doesn't exist)
    // O_WRONLY: open for writing only
    // O_CREAT: create the file if it doesn't exist
    // O_TRUNC: truncate the file to 0 length if it exists
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // Check for errors while opening the file
    if (fd == -1) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Redirect standard output (stdout) to the file
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("Failed to redirect stdout");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Close the file descriptor, as it's n
