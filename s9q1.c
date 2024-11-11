#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];  // Pipe file descriptors
    pid_t pid;
    char write_msg[] = "Hello from parent process!";
    char read_msg[100];

    // Create an unnamed pipe
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // Fork the child process
    pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {  // Child process
        // Close the write end of the pipe since we are only reading
        close(pipefd[1]);

        // Read from the pipe
        read(pipefd[0], read_msg, sizeof(read_msg));

        // Print the message received from the parent process
        printf("Child received: %s\n", read_msg);

        // Close the read end of the pipe
        close(pipefd[0]);
    } else {  // Parent process
        // Close the read end of the pipe since we are only writing
        close(pipefd[0]);

        // Write to the pipe
        write(pipefd[1], write_msg, strlen(write_msg) + 1);  // +1 to include the null terminator

        // Close the write end of the pipe
        close(pipefd[1]);

        // Optionally, the parent can also read from the pipe after writing, if needed
        // Read from the pipe (if desired)
        // read(pipefd[0], read_msg, sizeof(read_msg));
        // printf("Parent read: %s\n", read_msg);
    }

    return 0;
}
