#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

pid_t child_pid = -1;

// Signal handler for SIGALRM (Timeout after 5 seconds)
void handle_alarm(int sig) {
    if (child_pid != -1) {
        printf("Child process did not complete within 5 seconds. Killing child process...\n");
        kill(child_pid, SIGKILL); // Kill the child process
    }
}

// Signal handler for SIGCHLD (Child process termination)
void handle_child_death(int sig) {
    int status;
    pid_t terminated_pid = wait(&status);  // Wait for child to terminate

    if (terminated_pid == child_pid) {
        if (WIFEXITED(status)) {
            printf("Child process terminated normally with exit status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
        } else {
            printf("Child process terminated in an unknown manner\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [arguments...]\n", argv[0]);
        return 1;
    }

    struct sigaction sa_alarm, sa_child;

    // Set up SIGALRM signal handler for timeout (5 seconds)
    memset(&sa_alarm, 0, sizeof(sa_alarm));
    sa_alarm.sa_handler = handle_alarm;
    sigaction(SIGALRM, &sa_alarm, NULL);

    // Set up SIGCHLD signal handler for child termination
    memset(&sa_child, 0, sizeof(sa_child));
    sa_child.sa_handler = handle_child_death;
    sigaction(SIGCHLD, &sa_child, NULL);

    // Create the child process using fork()
    child_pid = fork();
    if (child_pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        // Child process: Execute the given command
        printf("Child process started: executing command '%s'\n", argv[1]);
        execvp(argv[1], &argv[1]);

        // If execvp fails
        perror("execvp failed");
        exit(1);
    } else {
        // Parent process: Set an alarm for 5 seconds
        printf("Parent process: waiting for child to complete or timeout\n");
        alarm(5); // Set the alarm to trigger after 5 seconds

        // Parent waits for child to terminate
        pause();  // Wait indefinitely for signals
    }

    return 0;
}
