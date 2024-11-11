#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // Open the file output.txt for writing (create if doesn't exist, truncate if exists)
    int file_desc = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_desc == -1) {
        perror("Error opening file");
        return 1;
    }

    // Redirect standard output (stdout) to the file
    if (dup2(file_desc, STDOUT_FILENO) == -1) {
        perror("Error redirecting output");
        close(file_desc);
        return 1;
    }

    // Now, any printf will go to output.txt instead of the terminal
    printf("This output will be written to the file output.txt\n");

    // Close the file descriptor after use
    close(file_desc);

    return 0;
}
