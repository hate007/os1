#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // Create a child process
    pid = fork();

    if (pid < 0) {
        // Error in fork
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {  // Child process
        // Simulate some work by the child process
        printf("Child process (PID: %d) started.\n", getpid());
        for (int i = 0; i < 10; i++) {
            printf("Child process is working... %d\n", i + 1);
            sleep(1);
        }
        printf("Child process completed work and is terminating.\n");
        exit(0);  // Child process terminates
    } else {  // Parent process
        // Parent process waits for 3 seconds before suspending the child
        sleep(3);

        printf("Parent process (PID: %d) suspending child process (PID: %d).\n", getpid(), pid);
        // Send SIGSTOP to suspend the child process
        kill(pid, SIGSTOP);
        
        // Wait for 3 seconds while the child is suspended
        sleep(3);

        printf("Parent process (PID: %d) resuming child process (PID: %d).\n", getpid(), pid);
        // Send SIGCONT to resume the child process
        kill(pid, SIGCONT);

        // Wait for the child to finish
        wait(NULL);
        printf("Parent process: Child process has terminated.\n");
    }

    return 0;
}
