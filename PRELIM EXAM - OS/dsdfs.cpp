#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <climits>
#include <queue>

using namespace std;



struct Process {
    int pid;      // Process ID
    int at;       // Arrival Time
    int bt;       // Burst Time
    int ct;       // Completion Time
    int tt;       // Turnaround Time
    int wt;       // Waiting Time
};



// Calculate recommended quantum time using mean
int recommendedQuantumTime(const vector<Process>& processes) {
    int sum = 0;
    for (const auto& p : processes) {
        sum += p.bt;
    }
    return sum / processes.size();
}

// Print scheduling results table
void printTable(vector<Process> processes, const string& algoName) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });
    
    cout << "\n" << algoName << "\n";
    cout << string(75, '=') << "\n";
    cout << left << setw(8) << "PID" 
         << setw(8) << "AT" 
         << setw(8) << "BT" 
         << setw(8) << "CT" 
         << setw(8) << "TT" 
         << setw(8) << "WT" << "\n";
    cout << string(75, '-') << "\n";
    
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
    
    cout << string(75, '-') << "\n";
    double avgTT = (double)totalTT / processes.size();
    double avgWT = (double)totalWT / processes.size();
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << avgTT << "\n";
    cout << "Average Waiting Time: " << fixed << setprecision(2) << avgWT << "\n";
}

// ============================================================================
// SCHEDULING ALGORITHMS
// ============================================================================

// SJF (Shortest Job First) - Non-preemptive
void sjf(vector<Process> processes) {
    int currentTime = 0;
    vector<bool> completed(processes.size(), false);
    int completedCount = 0;
    
    while (completedCount < processes.size()) {
        int minIdx = -1;
        int minBT = INT_MAX;
        
        // Find shortest job among arrived processes
        for (int i = 0; i < processes.size(); i++) {
            if (!completed[i] && processes[i].at <= currentTime && processes[i].bt < minBT) {
                minBT = processes[i].bt;
                minIdx = i;
            }
        }
        
        // If no process available, jump to next arrival
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
        
        // Execute the process
        currentTime += processes[minIdx].bt;
        processes[minIdx].ct = currentTime;
        processes[minIdx].tt = processes[minIdx].ct - processes[minIdx].at;
        processes[minIdx].wt = processes[minIdx].tt - processes[minIdx].bt;
        completed[minIdx] = true;
        completedCount++;
    }
    
    printTable(processes, "SJF (Shortest Job First)");
}

// SRTF (Shortest Remaining Time First) - Preemptive
void srtf(vector<Process> processes) {
    int currentTime = 0;
    int completedCount = 0;
    vector<int> remainingTime(processes.size());
    
    for (int i = 0; i < processes.size(); i++) {
        remainingTime[i] = processes[i].bt;
    }
    
    while (completedCount < processes.size()) {
        int minIdx = -1;
        int minRemaining = INT_MAX;
        
        // Find process with shortest remaining time
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].at <= currentTime && remainingTime[i] > 0 && remainingTime[i] < minRemaining) {
                minRemaining = remainingTime[i];
                minIdx = i;
            }
        }
        
        // If no process available, jump to next arrival
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
        
        // Execute for 1 unit of time
        remainingTime[minIdx]--;
        currentTime++;
        
        // If process completes
        if (remainingTime[minIdx] == 0) {
            processes[minIdx].ct = currentTime;
            processes[minIdx].tt = processes[minIdx].ct - processes[minIdx].at;
            processes[minIdx].wt = processes[minIdx].tt - processes[minIdx].bt;
            completedCount++;
        }
    }
    
    printTable(processes, "SRTF (Shortest Remaining Time First)");
}

// Round Robin - Time-sharing
void roundRobin(vector<Process> processes, int quantum) {
    int currentTime = 0;
    int completedCount = 0;
    vector<int> remainingTime(processes.size());
    vector<bool> inQueue(processes.size(), false);
    
    for (int i = 0; i < processes.size(); i++) {
        remainingTime[i] = processes[i].bt;
    }
    
    queue<int> q;
    
    while (completedCount < processes.size()) {
        // Add newly arrived processes
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].at <= currentTime && remainingTime[i] > 0 && !inQueue[i]) {
                q.push(i);
                inQueue[i] = true;
            }
        }
        
        // If queue empty, jump to next arrival
        if (q.empty()) {
            int nextArrival = INT_MAX;
            for (int i = 0; i < processes.size(); i++) {
                if (remainingTime[i] > 0) {
                    nextArrival = min(nextArrival, processes[i].at);
                }
            }
            currentTime = nextArrival;
            
            for (int i = 0; i < processes.size(); i++) {
                if (processes[i].at <= currentTime && remainingTime[i] > 0 && !inQueue[i]) {
                    q.push(i);
                    inQueue[i] = true;
                }
            }
            continue;
        }
        
        // Process front of queue
        int idx = q.front();
        q.pop();
        
        int timeSlice = min(quantum, remainingTime[idx]);
        currentTime += timeSlice;
        remainingTime[idx] -= timeSlice;
        
        if (remainingTime[idx] == 0) {
            processes[idx].ct = currentTime;
            processes[idx].tt = processes[idx].ct - processes[idx].at;
            processes[idx].wt = processes[idx].tt - processes[idx].bt;
            completedCount++;
        } else {
            q.push(idx);
        }
    }
    
    printTable(processes, "Round Robin (RR)");
}


int main() {
    vector<int> arrivalTimes = {1, 3, 8, 4, 6, 7, 2, 5};
    vector<int> burstTimes = {53, 43, 18, 16, 24, 73, 99, 27};
    
    // Create process list
    vector<Process> processes;
    for (int i = 0; i < arrivalTimes.size(); i++) {
        processes.push_back({i + 1, arrivalTimes[i], burstTimes[i], 0, 0, 0});
    }
    
    // Calculate recommended quantum time
    int quantum = recommendedQuantumTime(processes);
    
    // Display header
    cout << string(75, '=') << "\n";
    cout << "PROCESS SCHEDULING ALGORITHMS\n";
    cout << string(75, '=') << "\n";
    cout << "\nRecommended Quantum Time (for Round Robin): " << quantum << "\n";
    
    // Run all three algorithms
    sjf(processes);
    srtf(processes);
    roundRobin(processes, quantum);
    
    cout << "\n" << string(75, '=') << "\n";
    
    return 0;
}
