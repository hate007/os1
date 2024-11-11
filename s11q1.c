#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

void get_and_set_rlimits() {
    struct rlimit lim;

    // Get current file descriptor limit
    if (getrlimit(RLIMIT_NOFILE, &lim) == -1) {
        perror("getrlimit for RLIMIT_NOFILE failed");
        exit(EXIT_FAILURE);
    }
    printf("Current file descriptor limit: %lu\n", lim.rlim_cur);
    printf("Maximum file descriptor limit: %lu\n", lim.rlim_max);

    // Set new file descriptor limit
    lim.rlim_cur = 2048;  // Setting the soft limit to 2048
    if (setrlimit(RLIMIT_NOFILE, &lim) == -1) {
        perror("setrlimit for RLIMIT_NOFILE failed");
        exit(EXIT_FAILURE);
    }
    printf("File descriptor limit set to: %lu\n", lim.rlim_cur);

    // Get current stack size limit
    if (getrlimit(RLIMIT_STACK, &lim) == -1) {
        perror("getrlimit for RLIMIT_STACK failed");
        exit(EXIT_FAILURE);
    }
    printf("Current stack size limit: %lu bytes\n", lim.rlim_cur);
    printf("Maximum stack size limit: %lu bytes\n", lim.rlim_max);

    // Set new stack size limit (e.g., 16 MB)
    lim.rlim_cur = 16 * 1024 * 1024;  // 16 MB
    if (setrlimit(RLIMIT_STACK, &lim) == -1) {
        perror("setrlimit for RLIMIT_STACK failed");
        exit(EXIT_FAILURE);
    }
    printf("Stack size limit set to: %lu bytes\n", lim.rlim_cur);
}

int main() {
    // Display the current and set resource limits for file descriptors and stack size
    get_and_set_rlimits();

    return 0;
}
