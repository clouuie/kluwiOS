#include <stdio.h>

int main()
{
    int bt[] = {9, 8, 7, 8, 7, 8, 2, 1, 2, 5};
    int at[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int n = sizeof(bt) / sizeof(bt[0]);
    int wt[20], tat[20], i, ct[20];
    float wtavg = 0, tatavg = 0;

    // Calculate completion time for each process
    ct[0] = at[0] + bt[0];
    wt[0] = ct[0] - at[0] - bt[0];
    tat[0] = ct[0] - at[0];

    for(i = 1; i < n; i++)
    {
        // Completion time = max(previous completion, arrival time) + burst time
        ct[i] = (ct[i-1] > at[i]) ? ct[i-1] + bt[i] : at[i] + bt[i];
        
        // Waiting time = completion time - arrival time - burst time
        wt[i] = ct[i] - at[i] - bt[i];
        
        // Turnaround time = completion time - arrival time
        tat[i] = ct[i] - at[i];
        
        wtavg += wt[i];
        tatavg += tat[i];
    }

    wtavg += wt[0];
    tatavg += tat[0];

    printf("\n\tPROCESS\tARRIVAL TIME\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\n");
    for(i = 0; i < n; i++)
    {
        printf("\tP%d\t%d\t\t%d\t\t%d\t\t%d\n", i+1, at[i], bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time -- %.2f", wtavg/n);
    printf("\nAverage Turnaround Time -- %.2f\n", tatavg/n);

    return 0;
}