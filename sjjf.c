#include <stdio.h>

int main() {
    int n = 5;
    float bt[5] = {50.54, 10.11, 33.32, 2.5, 101.1};
    float wt[5], tat[5];
    int i, j;
    float temp;

    // Sort burst times (SJF)
    for (i = 0; i < n-1; i++) {
        for (j = i+1; j < n; j++) {
            if (bt[i] > bt[j]) {
                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;
            }
        }
    }

    wt[0] = 0;
    for (i = 1; i < n; i++) {
        wt[i] = wt[i-1] + bt[i-1];
    }

    float avg_wt = 0, avg_tat = 0;
    for (i = 0; i < n; i++) {
        tat[i] = wt[i] + bt[i];
        avg_wt += wt[i];
        avg_tat += tat[i];
    }

    avg_wt /= n;
    avg_tat /= n;

    printf("Process\tBT\tWT\tTAT\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%.2f\t%.2f\t%.2f\n", i+1, bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time = %.2f", avg_wt);
    printf("\nAverage Turnaround Time = %.2f\n", avg_tat);

    return 0;
}
