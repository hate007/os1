#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // Open a file for writing (create the file if it does not exist)
    int file_desc = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_desc == -1) {
        perror("Error opening file");
        return 1;
    }

    // Redirect the standard output (stdout) to the file
    if (dup2(file_desc, STDOUT_FILENO) == -1) {
        perror("Error redirecting output");
        return 1;
    }

    // Now, anything printed with printf() will be written to output.txt
    printf("This output will be written to the file output.txt\n");
    
    // Close the file descriptor
    close(file_desc);

    return 0;
}
