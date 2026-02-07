#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

    printf("=== Parent Process ===\n");
    printf("Parent PID: %d\n\n", getpid());

    pid1 = fork();

    if (pid1 == 0) {
        printf("Child 1 PID: %d (created by Parent)\n", getpid());
        sleep(1);

        pid2 = fork();

        if (pid2 == 0) {
            printf("Child 2 PID: %d (created by Child 1)\n", getpid());
            sleep(1);

            pid_t pid3 = fork();

            if (pid3 == 0) {
                printf("Child 3 PID: %d (created by Child 2)\n", getpid());
                sleep(1);
                printf("Child 3 finished\n");
                exit(0);
            }

            wait(NULL);
            printf("Child 2 finished (Child 3 completed)\n");
            exit(0);
        }

        wait(NULL);
        printf("Child 1 finished (Child 2 completed)\n");
        exit(0);
    }

    wait(NULL);
    printf("Parent finished (All children completed)\n");

    return 0;
}