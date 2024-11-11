#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

pid_t child_pid = -1;  // To store child process ID

// Signal handler for child process termination (SIGCHLD)
void handle_child_death(int sig) {
    int status;
    // Wait for child process to terminate and retrieve the status
    waitpid(child_pid, &status, 0);

    if (WIFEXITED(status)) {
        printf("Child process exited normally with status %d.\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child process terminated by signal %d.\n", WTERMSIG(status));
    }

    exit(0);  // Parent can exit after child finishes
}

// Signal handler for the alarm (SIGALRM)
void handle_alarm(int sig) {
    printf("Timeout reached. Child process is being killed.\n");
    kill(child_pid, SIGKILL);  // Kill the child process if it hasn't finished
    exit(1);  // Exit the parent after killing the child
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    // Fork a child process to run the user-defined command
    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        // In child process: execute the user command
        printf("Child process started. Executing: ");
        for (int i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");

        // Execute the command in the child process
        execvp(argv[1], &argv[1]);
        
        // If execvp fails, print an error
        perror("execvp failed");
        exit(1);
    } else {
        // In parent process: set up signal handlers and handle timeout

        // Set up signal handler for child process death (SIGCHLD)
        signal(SIGCHLD, handle_child_death);

        // Set up signal handler for alarm (SIGALRM)
        signal(SIGALRM, handle_alarm);

        // Set an alarm for 5 seconds
        alarm(5);

        // Wait for child process to complete, or timeout
        printf("Parent process: Waiting for the child to finish or timeout...\n");

        // Pause the parent process to wait for signals (SIGCHLD or SIGALRM)
        pause();
    }

    return 0;
}

