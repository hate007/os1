#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];  // Array to hold the pipe file descriptors
    pid_t pid;
    char buffer[100];  // Buffer to hold messages read from the pipe

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // Create a child process
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process - write messages to the pipe
        close(pipefd[0]);  // Close the read end of the pipe in the child

        // Write messages to the pipe
        write(pipefd[1], "Hello World", strlen("Hello World") + 1);
        write(pipefd[1], "Hello SPPU", strlen("Hello SPPU") + 1);
        write(pipefd[1], "Linux is Funny", strlen("Linux is Funny") + 1);

        close(pipefd[1]);  // Close the write end after writing
        exit(0);  // Exit child process
    } else {
        // Parent process - read messages from the pipe
        close(pipefd[1]);  // Close the write end of the pipe in the parent

        // Read and display the messages from the pipe
        printf("Messages from the child process:\n");
        while (read(pipefd[0], buffer, sizeof(buffer)) > 0) {
            printf("%s\n", buffer);  // Print each message received from the pipe
        }

        close(pipefd[0]);  // Close the read end after reading
        wait(NULL);  // Wait for the child process to finish
        printf("Parent process terminating.\n");
    }

    return 0;
}
