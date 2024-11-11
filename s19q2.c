#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];  // Pipe file descriptors

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork the first child process to run "ls -l"
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {
        // In the first child process
        // Close the unused read end of the pipe
        close(pipefd[0]);

        // Redirect stdout to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);  // No longer needed after dup2

        // Execute "ls -l" using execvp
        char *args[] = {"ls", "-l", NULL};
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    }

    // Fork the second child process to run "wc -l"
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        return 1;
    }

    if (pid2 == 0) {
        // In the second child process
        // Close the unused write end of the pipe
        close(pipefd[1]);

        // Redirect stdin to the read end of the pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);  // No longer needed after dup2

        // Execute "wc -l" using execvp
        char *args[] = {"wc", "-l", NULL};
        execvp(args[0], args);
        perror("execvp");
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
