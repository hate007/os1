#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void handle_signal(int sig) {
    // Child process will continue executing without any additional handling.
    // We are not using this handler in the child process because the main focus is
    // suspending and resuming the process from the parent side.
}

int main() {
    pid_t pid;
    
    // Fork to create a child process
    pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process code
        signal(SIGSTOP, handle_signal);  // Handle SIGSTOP in the child (not really needed)
        signal(SIGCONT, handle_signal);  // Handle SIGCONT in the child (not really needed)

        // Perform a task (print numbers in an infinite loop)
        int count = 0;
        while (1) {
            printf("Child process: %d\n", count++);
            sleep(1);  // Simulate work
        }
    } else {
        // Parent process code
        printf("Parent: Created child process with PID: %d\n", pid);

        // Let the child run for 3 seconds
        sleep(3);

        // Suspend the child process by sending SIGSTOP
        printf("Parent: Suspending the child process...\n");
        kill(pid, SIGSTOP);

        // Wait for 3 seconds before resuming the child
        sleep(3);

        // Resume the child process by sending SIGCONT
        printf("Parent: Resuming the child process...\n");
        kill(pid, SIGCONT);

        // Parent will wait for the child process to finish
        wait(NULL);
    }

    return 0;
}
