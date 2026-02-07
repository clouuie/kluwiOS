#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>

using namespace std;

struct ProcessExecution {
    int pid;
    float startTime;
    float endTime;
};

int main()
{
    float bt[] = {53, 43,};
    int n = sizeof(bt) / sizeof(bt[0]);
    float wt[20], tat[20];
    int i, pid[20];
    float ct[20];
    float wtavg = 0, tatavg = 0;
    int tq = 25; // Quantum time
    
    // Initialize process and remaining burst times
    for(i = 0; i < n; i++)
        pid[i] = i + 1;
    
    float rbt[20]; // Remaining burst time
    for(i = 0; i < n; i++)
        rbt[i] = bt[i];
    
    queue<int> q;
    float currentTime = 0;
    vector<ProcessExecution> gantt;
    
    // Add all processes to queue
    for(i = 0; i < n; i++)
        q.push(i);
    
    // Round Robin scheduling
    while(!q.empty())
    {
        int idx = q.front();
        q.pop();
        float startTime = currentTime;
        
        if(rbt[idx] <= tq)
        {
            currentTime += rbt[idx];
            ct[idx] = currentTime;
            rbt[idx] = 0;
        }
        else
        {
            currentTime += tq;
            rbt[idx] -= tq;
            q.push(idx);
        }
        
        gantt.push_back({pid[idx], startTime, currentTime});
    }
    
    // Calculate waiting and turnaround times
    for(i = 0; i < n; i++)
    {
        tat[i] = ct[i];
        wt[i] = tat[i] - bt[i];
        wtavg += wt[i];
        tatavg += tat[i];
    }
    
    // Print Gantt Chart
    cout << "\n\tGANTT CHART\n";
    cout << "\t";
    for(const auto& exec : gantt)
        cout << "| P" << exec.pid << " ";
    cout << "|\n\t";
    for(const auto& exec : gantt)
        cout << "|---";
    cout << "|\n\t";
    for(const auto& exec : gantt)
        cout << fixed << setprecision(1) << exec.startTime << "\t";
    cout << fixed << setprecision(1) << gantt.back().endTime << "\n";
    
    cout << "\n\tPROCESS\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\n";
    for(i = 0; i < n; i++)
    {
        cout << "\tP" << pid[i] << "\t" 
             << fixed << setprecision(2) << bt[i] << "\t\t" 
             << wt[i] << "\t\t" << tat[i] << "\n";
    }
    
    cout << "\nAverage Waiting Time -- " << fixed << setprecision(2) << wtavg/n;
    cout << "\nAverage Turnaround Time -- " << fixed << setprecision(2) << tatavg/n << "\n";
    
    return 0;
}
