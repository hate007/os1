#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int status;

    // Fork the process
    pid = fork();

    if (pid == -1) {
        // If fork fails, print an error and exit
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        // Simulate some work in the child process
        printf("Child process (PID: %d) is exiting with status 42.\n", getpid());
        exit(42);  // Child exits with status 42
    } else {  // Parent process
        // Parent waits for the child to terminate and retrieve the exit status
        waitpid(pid, &status, 0);

        // Check if the child terminated normally
        if (WIFEXITED(status)) {
            printf("Child process (PID: %d) exited normally with exit status: %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            // If the child terminated due to a signal
            printf("Child process (PID: %d) was terminated by signal: %d\n", pid, WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            // If the child process was stopped (not a common case in this simple program)
            printf("Child process (PID: %d) was stopped by signal: %d\n", pid, WSTOPSIG(status));
        } else {
            printf("Unknown status for child process (PID: %d)\n", pid);
        }
    }

    return 0;
}
