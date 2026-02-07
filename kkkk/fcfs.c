#include <stdio.h>

int main(void)
{
    int n, i;
    int at[20], bt[20], wt[20], tat[20];
    int st[20], ct[20];
    int total_bt = 0, idle_time = 0;

    float wtavg = 0, tatavg = 0, cpu_util;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        printf("\nProcess %d Arrival Time: ", i + 1);
        scanf("%d", &at[i]);

        printf("Process %d Burst Time: ", i + 1);
        scanf("%d", &bt[i]);

        total_bt += bt[i];
    }

    /* First process */
    st[0] = at[0];
    ct[0] = st[0] + bt[0];
    wt[0] = 0;
    tat[0] = bt[0];

    /* Remaining processes */
    for (i = 1; i < n; i++)
    {
        if (ct[i - 1] < at[i])
        {
            idle_time += at[i] - ct[i - 1];
            st[i] = at[i];
        }
        else
        {
            st[i] = ct[i - 1];
        }

        ct[i] = st[i] + bt[i];
        wt[i] = st[i] - at[i];
        tat[i] = ct[i] - at[i];
    }

    for (i = 0; i < n; i++)
    {
        wtavg += wt[i];
        tatavg += tat[i];
    }

    cpu_util = ((float)total_bt / (ct[n - 1] - at[0])) * 100;

    printf("\nPROCESS\tAT\tBT\tWT\tTAT\n");
    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\n",
               i + 1, at[i], bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time = %.2f", wtavg / n);
    printf("\nAverage Turnaround Time = %.2f", tatavg / n);
    printf("\nCPU Utilization = %.2f%%\n", cpu_util);

    return 0;
}
