#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipefd[2];  // Pipe file descriptors
    pid_t pid1, pid2;

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // Fork the first child to execute 'ls -l'
    if ((pid1 = fork()) == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid1 == 0) {  // First child process
        // Close the read end of the pipe since we are only writing
        close(pipefd[0]);

        // Redirect stdout to the write end of the pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2 failed");
            exit(1);
        }

        // Close the write end of the pipe after redirecting
        close(pipefd[1]);

        // Execute 'ls -l'
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp failed");  // If exec fails, print error
        exit(1);
    }

    // Fork the second child to execute 'wc -l'
    if ((pid2 = fork()) == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid2 == 0) {  // Second child process
        // Close the write end of the pipe since we are only reading
        close(pipefd[1]);

        // Redirect stdin to the read end of the pipe
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2 failed");
            exit(1);
        }

        // Close the read end of the pipe after redirecting
        close(pipefd[0]);

        // Execute 'wc -l'
        execlp("wc", "wc", "-l", (char *)NULL);
        perror("execlp failed");  // If exec fails, print error
        exit(1);
    }

    // Parent process closes both ends of the pipe
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
