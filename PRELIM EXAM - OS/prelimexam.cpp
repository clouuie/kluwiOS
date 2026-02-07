#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <climits>

using namespace std;

// ============================================================================
// DATA STRUCTURES
// ============================================================================

// Process structure - represents a single process with scheduling metrics
struct Process {
    int pid;      // Process ID
    int at;       // Arrival Time
    int bt;       // Burst Time
    int ct;       // Completion Time
    int tt;       // Turnaround Time
    int wt;       // Waiting Time
};

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================

// Forward declarations
void roundRobin(vector<Process> processes, int quantum);

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Function to calculate recommended quantum time (using mean)
// Used for Round Robin scheduling to determine time slice
int recommendedQuantumTime(vector<Process>& processes) {
    int sum = 0;
    for (const auto& p : processes) {
        sum += p.bt;
    }
    return sum / processes.size();
}

// Function to print table - displays scheduling results with statistics
// Calculates and displays average turnaround time and waiting time
void printTable(vector<Process>& processes, const string& algoName) {
    cout << "\n" << algoName << "\n";
    cout << string(70, '=') << "\n";
    cout << left << setw(8) << "PID" 
         << setw(8) << "AT" 
         << setw(8) << "BT" 
         << setw(8) << "CT" 
         << setw(8) << "TT" 
         << setw(8) << "WT" << "\n";
    cout << string(70, '-') << "\n";
    
    int totalTT = 0, totalWT = 0;
    for (const auto& p : processes) {
        cout << left << setw(8) << p.pid 
             << setw(8) << p.at 
             << setw(8) << p.bt 
             << setw(8) << p.ct 
             << setw(8) << p.tt 
             << setw(8) << p.wt << "\n";
        totalTT += p.tt;
        totalWT += p.wt;
    }
    
    cout << string(70, '-') << "\n";
    double avgTT = (double)totalTT / processes.size();
    double avgWT = (double)totalWT / processes.size();
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << avgTT << "\n";
    cout << "Average Waiting Time: " << fixed << setprecision(2) << avgWT << "\n";
}

// ============================================================================
// SCHEDULING ALGORITHMS
// ============================================================================

// SJF (Shortest Job First) - Non-preemptive scheduling algorithm
// Selects process with shortest burst time among available processes
void sjf(vector<Process> processes) {
    int currentTime = 0;
    vector<bool> completed(processes.size(), false);
    int completedCount = 0;
    
    // Process all processes
    while (completedCount < processes.size()) {
        int minIdx = -1;
        int minBT = INT_MAX;
        
        // Find process with shortest burst time that has arrived
        for (int i = 0; i < processes.size(); i++) {
            if (!completed[i] && processes[i].at <= currentTime && processes[i].bt < minBT) {
                minBT = processes[i].bt;
                minIdx = i;
            }
        }
        
        // If no process available, jump to next arrival time
        if (minIdx == -1) {
            int nextArrival = INT_MAX;
            for (int i = 0; i < processes.size(); i++) {
                if (!completed[i]) {
                    nextArrival = min(nextArrival, processes[i].at);
                }
            }
            currentTime = nextArrival;
            continue;
        }
        
        // Execute the selected process
        currentTime += processes[minIdx].bt;
        processes[minIdx].ct = currentTime;
        processes[minIdx].tt = processes[minIdx].ct - processes[minIdx].at;
        processes[minIdx].wt = processes[minIdx].tt - processes[minIdx].bt;
        completed[minIdx] = true;
        completedCount++;
    }
    
    // Sort back by PID for display
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });
    
    printTable(processes, "SJF (Shortest Job First)");
}

// SRTF (Shortest Remaining Time First) - Preemptive scheduling algorithm
// Executes process with shortest remaining time, can switch between processes
void srtf(vector<Process> processes) {
    int currentTime = 0;
    int completedCount = 0;
    vector<int> remainingTime(processes.size());
    
    // Initialize remaining time for each process
    for (int i = 0; i < processes.size(); i++) {
        remainingTime[i] = processes[i].bt;
    }
    
    // Process all processes one unit at a time
    while (completedCount < processes.size()) {
        int minIdx = -1;
        int minRemaining = INT_MAX;
        
        // Find process with shortest remaining time that has arrived
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].at <= currentTime && remainingTime[i] > 0 && remainingTime[i] < minRemaining) {
                minRemaining = remainingTime[i];
                minIdx = i;
            }
        }
        
        // If no process available, jump to next arrival time
        if (minIdx == -1) {
            int nextArrival = INT_MAX;
            for (int i = 0; i < processes.size(); i++) {
                if (remainingTime[i] > 0) {
                    nextArrival = min(nextArrival, processes[i].at);
                }
            }
            currentTime = nextArrival;
            continue;
        }
        
        // Execute one unit of time for selected process
        remainingTime[minIdx]--;
        currentTime++;
        
        // Mark process as complete if remaining time is zero
        if (remainingTime[minIdx] == 0) {
            processes[minIdx].ct = currentTime;
            processes[minIdx].tt = processes[minIdx].ct - processes[minIdx].at;
            processes[minIdx].wt = processes[minIdx].tt - processes[minIdx].bt;
            completedCount++;
        }
    }
    
    // Sort back by PID for display
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });
    
    printTable(processes, "SRTF (Shortest Remaining Time First)");
}

// Round Robin - Time-sharing scheduling algorithm
// Each process gets a fixed time quantum, then goes to end of queue
void roundRobin(vector<Process> processes, int quantum) {
    int currentTime = 0;
    int completedCount = 0;
    vector<int> remainingTime(processes.size());
    vector<bool> inQueue(processes.size(), false);
    
    // Initialize remaining time for each process
    for (int i = 0; i < processes.size(); i++) {
        remainingTime[i] = processes[i].bt;
    }
    
    vector<int> queue;
    
    // Process all processes using time quantum
    while (completedCount < processes.size()) {
        // Add newly arrived processes to queue
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].at <= currentTime && remainingTime[i] > 0 && !inQueue[i]) {
                queue.push_back(i);
                inQueue[i] = true;
            }
        }
        
        // If queue is empty, jump to next arrival time
        if (queue.empty()) {
            int nextArrival = INT_MAX;
            for (int i = 0; i < processes.size(); i++) {
                if (remainingTime[i] > 0) {
                    nextArrival = min(nextArrival, processes[i].at);
                }
            }
            currentTime = nextArrival;
            
            for (int i = 0; i < processes.size(); i++) {
                if (processes[i].at <= currentTime && remainingTime[i] > 0 && !inQueue[i]) {
                    queue.push_back(i);
                    inQueue[i] = true;
                }
            }
            continue;
        }
        
        // Execute front process for one time quantum
        int idx = queue.front();
        queue.erase(queue.begin());
        
        int timeSlice = min(quantum, remainingTime[idx]);
        currentTime += timeSlice;
        remainingTime[idx] -= timeSlice;
        
        // Mark as complete or re-queue if time remains
        if (remainingTime[idx] == 0) {
            processes[idx].ct = currentTime;
            processes[idx].tt = processes[idx].ct - processes[idx].at;
            processes[idx].wt = processes[idx].tt - processes[idx].bt;
            completedCount++;
        } else {
            queue.push_back(idx);
        }
    }
    
    // Sort back by PID for display
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });
    
    printTable(processes, "Round Robin (RR)");
}

// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main() {
    // Initialize test data with arrival times and burst times
    vector<int> arrivalTimes = {1, 3, 8, 4, 6, 7, 2, 5};
    vector<int> burstTimes = {53, 43, 18, 16, 24, 73, 88, 27};
    
    // Create process list from input data
    vector<Process> processes;
    for (int i = 0; i < arrivalTimes.size(); i++) {
        processes.push_back({i + 1, arrivalTimes[i], burstTimes[i], 0, 0, 0});
    }
    
    // Calculate recommended quantum time for Round Robin
    int quantum = recommendedQuantumTime(processes);
    
    // Display header and quantum time
    cout << string(70, '=') << "\n";
    cout << "PROCESS SCHEDULING ALGORITHMS\n";
    cout << string(70, '=') << "\n";
    cout << "\nRecommended Quantum Time (for Round Robin): " << quantum << "\n";
    
    // Run all three scheduling algorithms
    sjf(processes);
    srtf(processes);
    roundRobin(processes, quantum);
    
    cout << "\n" << string(70, '=') << "\n";
    
    return 0;
}
