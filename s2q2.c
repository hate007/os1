#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int sigint_count = 0;  

void handle_sigint(int sig) {
    sigint_count++;  
    if (sigint_count == 1) {
        printf("\nCaught SIGINT (Ctrl+C). Press Ctrl+C again to exit.\n");
        signal(SIGINT, SIG_DFL);
    } else if (sigint_count == 2) {
        printf("\nExiting the program on second Ctrl+C...\n");
    }
}

int main() {
    signal(SIGINT, handle_sigint);

    while (1) {
        printf("Program is running... Press Ctrl+C to interrupt.\n");
        sleep(2);
    }
}
