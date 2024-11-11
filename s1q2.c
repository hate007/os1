#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_alarm(int sig) {
    printf("Alarm is fired!\n");
}

int main() {
    signal(SIGALRM, handle_alarm);

    pid_t pid = fork();

    if (pid == 0) {
        sleep(2); 
        printf("Child sending SIGALRM to parent...\n");
        kill(getppid(), SIGALRM);
    } else {
        printf("Parent waiting for the alarm signal...\n");
        sleep(5); 
    }

}
