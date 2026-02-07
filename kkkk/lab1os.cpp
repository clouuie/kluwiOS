#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>

class ProcessInfo {
private:
    pid_t pid;
    pid_t ppid;
    std::string user;
    int userId;
    int groupId;

public:
    ProcessInfo() {
        pid = getpid();
        ppid = getppid();
        const char* username = getenv("USER");
        user = username ? username : "unknown";
        userId = 1000;
        groupId = 1000;
    }

    void display() {
        std::cout << "Process ID is: " << pid << std::endl;
        std::cout << "User: " << user << std::endl << std::endl;
        std::cout << "=== Additional Information ===" << std::endl;
        std::cout << "Parent Process ID: " << ppid << std::endl;
        std::cout << "User ID: " << userId << std::endl;
        std::cout << "Group ID: " << groupId << std::endl;
    }
};

int main() {
    ProcessInfo p;
    p.display();
    return 0;
}