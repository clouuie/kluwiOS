#include <iostream> 
#include <cstdlib> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 

using namespace std;

int main() { 
    pid_t pid1, pid2; 

    pid1 = fork(); // Create first child process; returns child PID to parent, 0 to child

    if (pid1 == 0) { // Execute if this is Child 1 (fork returned 0)
        pid2 = fork(); // Child 1 creates its own child (Child 2)

        if (pid2 == 0) { // Execute if this is Child 2 (fork returned 0)
            pid_t pid3 = fork(); // Child 2 creates its own child (Child 3)

            if (pid3 == 0) { // Execute if this is Child 3 (fork returned 0)
                pid_t pid4 = fork(); // Child 3 creates its own child (Child 4)

                if (pid4 == 0) { // Execute if this is Child 4 (fork returned 0)
                    exit(0); // Child 4 terminates
                }

                wait(NULL); // Child 3 waits for Child 4 to finish
                cout << "Child 4 PID: " << getpid() << " (created by Child 3)" << endl; // Print Child 3's PID
                exit(0); // Child 3 terminates
            }

            wait(NULL); // Child 2 waits for Child 3 to finish
            cout << "Child 3 PID: " << getpid() << " (created by Child 2)" << endl; // Print Child 2's PID
            exit(0); // Child 2 terminates
        }

        wait(NULL); // Child 1 waits for Child 2 to finish
        cout << "Child 2 PID: " << getpid() << " (created by Child 1)" << endl; // Print Child 1's PID
        exit(0); // Child 1 terminates
    }

    wait(NULL); // Parent waits for Child 1 to finish
    cout << "Child 1 PID: " << getpid() << " (created by Parent)" << endl; // Print Parent's PID (labeled as Child 1)
    cout << "Parent PID: " << getpid() << endl; // Print Parent's PID again

    return 0; 
}
