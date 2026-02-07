#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

    pid1 = fork();   // First child

    if (pid1 == 0) {
        // Child 1
        printf("Child 1: PID = %d, Parent PID = %d\n", getpid(), getppid());
    } 
    else {
        pid2 = fork();  // Second child

        if (pid2 == 0) {
            // Child 2
            printf("Child 2: PID = %d, Parent PID = %d\n", getpid(), getppid());
        } 
        else {
            // Parent process
            printf("Parent: PID = %d\n", getpid());

            // Parent waits for the last child to finish
            wait(NULL);
            wait(NULL);

            printf("Parent: All child processes finished.\n");
        }
    }
    return 0;
}
