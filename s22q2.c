#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

void block_signals() {
    // Block SIGINT (Ctrl-C) and SIGQUIT (Ctrl-\)
    sigset_t new_set;
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGINT);
    sigaddset(&new_set, SIGQUIT);
    sigprocmask(SIG_BLOCK, &new_set, NULL);
}

int main() {
    int pipe_fd[2];  // Pipe file descriptors

    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1;
    }

    // Block the SIGINT and SIGQUIT signals
    block_signals();

    // Fork the first child process (for 'ls -l')
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {
        // In child 1: Execute 'ls -l'
        close(pipe_fd[0]);  // Close the read end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);  // Redirect stdout to pipe
        close(pipe_fd[1]);  // Close the write end after redirect

        // Execute 'ls -l'
        execlp("ls", "ls", "-l", NULL);
        perror("execlp(ls)");
        exit(1);  // If execlp fails
    }

    // Fork the second child process (for 'wc -l')
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        return 1;
    }

    if (pid2 == 0) {
        // In child 2: Execute 'wc -l'
        close(pipe_fd[1]);  // Close the write end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO);  // Redirect stdin from pipe
        close(pipe_fd[0]);  // Close the read end after redirect

        // Execute 'wc -l'
        execlp("wc", "wc", "-l", NULL);
        perror("execlp(wc)");
        exit(1);  // If execlp fails
    }

    // Parent process: Close both ends of the pipe
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // Wait for both child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
