#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

pid_t child_pid = -1;  // To store the child process ID

// Signal handler for child process death (SIGCHLD)
void handle_child_death(int sig) {
    int status;
    // Wait for the child to finish, return the child's exit status
    if (waitpid(child_pid, &status, WNOHANG) > 0) {
        if (WIFEXITED(status)) {
            printf("Child process terminated with exit status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
        }
    }
}

// Signal handler for alarm signal (SIGALRM)
void handle_alarm(int sig) {
    printf("Alarm triggered: Child process has not finished in 5 seconds, killing the child...\n");
    if (child_pid != -1) {
        kill(child_pid, SIGKILL);  // Kill the child process
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    // Register signal handlers
    signal(SIGCHLD, handle_child_death);  // Handle child termination
    signal(SIGALRM, handle_alarm);        // Handle alarm signal

    // Create a child process using fork()
    child_pid = fork();

    if (child_pid == -1) {
        // Fork failed
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        // Child process: Execute the command
        printf("Child process running command: ");
        for (int i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");

        // Execute the command using execvp()
        if (execvp(argv[1], &argv[1]) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else {
        // Parent process
        printf("Parent process: Setting alarm for 5 seconds...\n");

        // Set an alarm to trigger in 5 seconds
        alarm(5);

        // Wait for the child process to complete
        int status;
        if (waitpid(child_pid, &status, 0) == -1) {
            perror("waitpid failed");
        }

        // In case child is still running after the alarm triggers, the signal handler will kill the child
    }

    return 0;
}
