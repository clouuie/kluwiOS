#include <stdio.h>

int main() 
{
    int p[20], bt[20], at[20], wt[20], tat[20], ct[20], i, k, n, temp;
    float wtavg, tatavg, cpu_util;
    int total_idle_time = 0;

    printf("\nEnter the number of processes -- ");
    if (scanf("%d", &n) != 1) return 1;

    for(i=0; i<n; i++) 
    {
        p[i] = i;
        printf("Enter Arrival Time for Process %d -- ", i);
        scanf("%d", &at[i]);
        printf("Enter Burst Time for Process %d -- ", i);
        scanf("%d", &bt[i]);
    }

    // Sorting processes by Arrival Time (and Burst Time as tie-breaker for SJF)
    for(i=0; i<n; i++) {
        for(k=i+1; k<n; k++) {
            if(at[i] > at[k] || (at[i] == at[k] && bt[i] > bt[k])) {
                // Swap Arrival Time
                temp = at[i]; at[i] = at[k]; at[k] = temp;
                // Swap Burst Time
                temp = bt[i]; bt[i] = bt[k]; bt[k] = temp;
                // Swap Process ID
                temp = p[i]; p[i] = p[k]; p[k] = temp;
            }
        }
    }

    // Calculations
    int current_time = 0;
    for(i=0; i<n; i++) {
        if (current_time < at[i]) {
            total_idle_time += (at[i] - current_time);
            current_time = at[i];
        }
        
        wt[i] = current_time - at[i];
        current_time += bt[i];
        ct[i] = current_time;
        tat[i] = ct[i] - at[i];
        
        wtavg += wt[i];
        tatavg += tat[i];
    }

    // CPU Utilization calculation
    // Formula: (Total Time - Idle Time) / Total Time
    int total_execution_time = ct[n-1];
    cpu_util = ((float)(total_execution_time - total_idle_time) / total_execution_time) * 100;

    printf("\n\t P \t AT \t BT \t WT \t TAT \t CT \n");
    for(i=0; i<n; i++) {
        printf("\t P%d \t %d \t %d \t %d \t %d \t %d\n", p[i], at[i], bt[i], wt[i], tat[i], ct[i]);
    }

    printf("\nAverage Waiting Time -- %.2f", wtavg/n);
    printf("\nAverage Turnaround Time -- %.2f", tatavg/n);
    printf("\nCPU Utilization -- %.2f%%", cpu_util);

    return 0;
}