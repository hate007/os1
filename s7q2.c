#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];  // Pipe file descriptors
    pid_t pid1, pid2;

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // Fork the first child to run 'ls -l'
    if ((pid1 = fork()) == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid1 == 0) {  // First child process
        // Close the write end of the pipe (we will only write to the pipe)
        close(pipefd[0]);

        // Redirect stdout to the pipe
        dup2(pipefd[1], STDOUT_FILENO);

        // Close the original write end of the pipe after duplicating
        close(pipefd[1]);

        // Execute 'ls -l'
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp failed");  // If exec fails, print error
        exit(1);
    }

    // Fork the second child to run 'wc -l'
    if ((pid2 = fork()) == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid2 == 0) {  // Second child process
        // Close the read end of the pipe (we will only read from the pipe)
        close(pipefd[1]);

        // Redirect stdin to read from the pipe
        dup2(pipefd[0], STDIN_FILENO);

        // Close the original read end of the pipe after duplicating
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
