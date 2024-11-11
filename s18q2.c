#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];  // Pipe file descriptors
    pid_t pid;
    char buffer[100];

    // Create an unnamed pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    // Fork the process to create a child
    pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process: Write messages to the pipe
        close(pipefd[0]);  // Close the read end of the pipe

        // Write three messages to the pipe
        write(pipefd[1], "Hello World\n", strlen("Hello World\n"));
        write(pipefd[1], "Hello SPPU\n", strlen("Hello SPPU\n"));
        write(pipefd[1], "Linux is Funny\n", strlen("Linux is Funny\n"));

        close(pipefd[1]);  // Close the write end after writing
        exit(0);  // Child process exits
    } else {
        // Parent process: Read messages from the pipe
        close(pipefd[1]);  // Close the write end of the pipe

        // Read and display messages from the pipe
        while (read(pipefd[0], buffer, sizeof(buffer)) > 0) {
            printf("Parent received: %s", buffer);  // Print the message read from the pipe
        }

        close(pipefd[0]);  // Close the read end after reading
        wait(NULL);  // Wait for the child process to finish
    }

    return 0;
}
