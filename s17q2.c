#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void block_signals() {
    sigset_t set;
    
    // Initialize the signal set
    sigemptyset(&set);

    // Block SIGINT (Ctrl-C) and SIGQUIT (Ctrl-\)
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    // Block the signals for the current process
    if (sigprocmask(SIG_BLOCK, &set, NULL) < 0) {
        perror("sigprocmask");
        exit(1);
    }
}

int main() {
    int pipefd[2];  // Pipe file descriptors
    pid_t pid1, pid2;

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    // Block SIGINT and SIGQUIT signals
    block_signals();

    // Fork the first child process (for 'ls -l')
    if ((pid1 = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    if (pid1 == 0) {
        // Child 1: Execute 'ls -l'
        // Close the unused write end of the pipe
        close(pipefd[0]);

        // Redirect standard output to the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Execute the 'ls -l' command
        execlp("ls", "ls", "-l", NULL);
        // If execlp fails
        perror("execlp");
        exit(1);
    }

    // Fork the second child process (for 'wc -l')
    if ((pid2 = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    if (pid2 == 0) {
        // Child 2: Execute 'wc -l'
        // Close the unused read end of the pipe
        close(pipefd[1]);

        // Redirect standard input to the pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        // Execute the 'wc -l' command
        execlp("wc", "wc", "-l", NULL);
        // If execlp fails
        perror("execlp");
        exit(1);
    }

    // Parent process: Close both ends of the pipe
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    // Unblock signals after child processes are done
    sigset_t set;
    sigemptyset(&set);
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    return 0;
}
