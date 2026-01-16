#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    float bt[] = {99.99, 107.9, 143.77, 20.24, 50.55, 66.66};
    int n = sizeof(bt) / sizeof(bt[0]);
    float wt[20], tat[20];
    int i, pid[20];
    float ct[20];
    float wtavg = 0, tatavg = 0;

    // Initialize process IDs
    for(i = 0; i < n; i++)
        pid[i] = i + 1;

    // Sort by burst time (SJF) 
    for(i = 0; i < n; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            if(bt[j] > bt[j+1])
            {
                // Swap burst times
                float temp = bt[j];
                bt[j] = bt[j+1];
                bt[j+1] = temp;
                
                // Swap process IDs
                int temppid = pid[j];
                pid[j] = pid[j+1];
                pid[j+1] = temppid;
            }
        }
    }

    // Calculate completion time for each process
    ct[0] = bt[0];
    wt[0] = 0;
    tat[0] = ct[0];

    for(i = 1; i < n; i++)
    {
        ct[i] = ct[i-1] + bt[i];
        wt[i] = ct[i] - bt[i];
        tat[i] = ct[i];
        
        wtavg += wt[i];
        tatavg += tat[i];
    }

    wtavg += wt[0];
    tatavg += tat[0];

    // Gantt Chart
    cout << "\n\tGANTT CHART\n";
    cout << "\t";
    for(i = 0; i < n; i++)
        cout << "-----";
    cout << "\n\t|";
    for(i = 0; i < n; i++)
        cout << " P" << pid[i] << " |";
    cout << "\n\t";
    for(i = 0; i < n; i++)
        cout << "-----";
    cout << "\n\t";
    cout << "0";
    for(i = 0; i < n; i++)
    {
        cout << setw(4) << fixed << setprecision(1) << ct[i];
    }
    cout << "\n";

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
