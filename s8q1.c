#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>  // For open flags
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    // Open the file for writing (creating it if it doesn't exist, or truncating if it does)
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    // Redirect standard output (stdout) to the file
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("Error redirecting stdout");
        close(fd);
        exit(1);
    }

    // Close the original file descriptor since we no longer need it
    close(fd);

    // Now printf will write to the file instead of the terminal
    printf("This will be written to output.txt instead of the terminal.\n");
    printf("All subsequent printf calls will also go to the file.\n");

    return 0;
}
