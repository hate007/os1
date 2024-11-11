#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

void handle_sighup(int sig) {
    printf("Child received SIGHUP signal\n");
}

void handle_sigint(int sig) {
    printf("Child received SIGINT signal\n");
}

void handle_sigquit(int sig) {
    // Print the message and exit when SIGQUIT is received
    printf("My DADDY has Killed me!!!\n");
    exit(0);
}

int main() {
    pid_t child_pid;

    // Create a child process
    child_pid = fork();

    if (child_pid == -1) {
        // Fork failed
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        // Child process
        // Set up signal handlers
        signal(SIGHUP, handle_sighup);
        signal(SIGINT, handle_sigint);
        signal(SIGQUIT, handle_sigquit);

        // Wait indefinitely for signals
        printf("Child process is waiting for signals...\n");
        while (1) {
            // Sleep to allow signal handling
            sleep(1);
        }
    } else {
        // Parent process
        // Send SIGHUP or SIGINT signal every 3 seconds
        for (int i = 0; i < 10; i++) {
            if (i % 2 == 0) {
                // Send SIGHUP
                kill(child_pid, SIGHUP);
            } else {
                // Send SIGINT
                kill(child_pid, SIGINT);
            }
            sleep(3);  // Wait for 3 seconds before sending the next signal
        }

        // After 30 seconds, send SIGQUIT to kill the child
        printf("Parent will send SIGQUIT to terminate the child...\n");
        sleep(30);  // Wait for 30 seconds
        kill(child_pid, SIGQUIT);

        // Wait for the child to terminate
        wait(NULL);
    }

    return 0;
}
