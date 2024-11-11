#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handle_sighup(int sig) {
    printf("Child received SIGHUP signal.\n");
}

void handle_sigint(int sig) {
    printf("Child received SIGINT signal.\n");
}

void handle_sigquit(int sig) {
    printf("Child received SIGQUIT signal. My Papa has Killed me!!!\n");
    exit(0);  // Exit the child process
}

int main() {
    pid_t pid = fork();  // Create a child process

    if (pid < 0) {
        // If fork() fails
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        // Set up signal handlers
        signal(SIGHUP, handle_sighup);
        signal(SIGINT, handle_sigint);
        signal(SIGQUIT, handle_sigquit);

        // Wait for signals
        while (1) {
            pause();  // Wait for any signal
        }
    } else {
        // Parent process
        sleep(3);  // Wait for 3 seconds before sending first signal

        // Send SIGHUP or SIGINT signal every 3 seconds for 12 seconds
        for (int i = 0; i < 4; i++) {
            sleep(3);  // Wait 3 seconds
            if (i % 2 == 0) {
                // Send SIGHUP signal on even iterations
                kill(pid, SIGHUP);
            } else {
                // Send SIGINT signal on odd iterations
                kill(pid, SIGINT);
            }
        }

        // After 15 seconds, send SIGQUIT to terminate the child
        sleep(3);  // Wait for another 3 seconds
        kill(pid, SIGQUIT);

        // Wait for the child process to terminate
        wait(NULL);
        printf("Parent process terminating.\n");
    }

    return 0;
}
