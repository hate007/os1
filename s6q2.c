#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

int main() {
    int n;
    printf("Enter the number of child processes to create: ");
    scanf("%d", &n);

    struct rusage usage;
    struct rusage total_usage = {0};  // Initialize total usage to zero

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) {
            // Child process
            // Simulate some work in the child process
            printf("Child %d is working...\n", getpid());
            for (int j = 0; j < 1000000; j++);  // Simple CPU-bound task

            exit(0);  // Child process terminates here
        }
    }

    // Parent process waits for all child processes
    for (int i = 0; i < n; i++) {
        pid_t pid = waitpid(-1, NULL, 0);  // Wait for any child process
        if (pid == -1) {
            perror("waitpid failed");
            exit(1);
        }

        // Get the resource usage of the child process
        if (getrusage(RUSAGE_CHILDREN, &usage) == -1) {
            perror("getrusage failed");
            exit(1);
        }

        // Add the child process usage to the total usage
        total_usage.ru_utime.tv_sec += usage.ru_utime.tv_sec;
        total_usage.ru_utime.tv_usec += usage.ru_utime.tv_usec;
        total_usage.ru_stime.tv_sec += usage.ru_stime.tv_sec;
        total_usage.ru_stime.tv_usec += usage.ru_stime.tv_usec;
    }

    // Handle microsecond overflow
    if (total_usage.ru_utime.tv_usec >= 1000000) {
        total_usage.ru_utime.tv_sec += total_usage.ru_utime.tv_usec / 1000000;
        total_usage.ru_utime.tv_usec %= 1000000;
    }
    if (total_usage.ru_stime.tv_usec >= 1000000) {
        total_usage.ru_stime.tv_sec += total_usage.ru_stime.tv_usec / 1000000;
        total_usage.ru_stime.tv_usec %= 1000000;
    }

    // Display the total cumulative time spent in user and kernel mode
    printf("\nTotal Cumulative Time Spent by Children Processes:\n");
    printf("User time: %ld seconds %ld microseconds\n", total_usage.ru_utime.tv_sec, total_usage.ru_utime.tv_usec);
    printf("Kernel time: %ld seconds %ld microseconds\n", total_usage.ru_stime.tv_sec, total_usage.ru_stime.tv_usec);

    return 0;
}
