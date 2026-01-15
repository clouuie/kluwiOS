#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100

struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

int main() {
    int n;
    struct process p[MAX_PROCESSES];
    float avg_turnaround_time;
    float avg_waiting_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_idle_time = 0;
    int is_completed[MAX_PROCESSES];
    
    // Initialize is_completed array to 0
    memset(is_completed, 0, sizeof(is_completed));

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
        printf("Enter arrival time of process %d: ", i + 1);
        scanf("%d", &p[i].arrival_time);
        printf("Enter burst time of process %d: ", i + 1);
        scanf("%d", &p[i].burst_time);
        printf("Enter priority of the process %d: ", i + 1);
        scanf("%d", &p[i].priority);
        p[i].pid = i + 1;
        printf("\n");
    }

    int current_time = 0;
    int completed = 0;
    int prev = 0;

    while(completed != n) {
        int idx = -1;
        int min_priority = 1000000; // Smaller number = higher priority
        
        // Find process with highest priority (lowest priority number)
        for(int i = 0; i < n; i++) {
            if(p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if(p[i].priority < min_priority) {
                    min_priority = p[i].priority;
                    idx = i;
                }
                else if(p[i].priority == min_priority) {
                    // Tie-breaker: earlier arrival time
                    if(p[i].arrival_time < p[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }
        
        if(idx != -1) {
            // Execute the selected process
            p[idx].start_time = current_time;
            p[idx].completion_time = p[idx].start_time + p[idx].burst_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            
            // Update totals
            total_turnaround_time += p[idx].turnaround_time;
            total_waiting_time += p[idx].waiting_time;
            total_idle_time += p[idx].start_time - prev;

            // Mark as completed
            is_completed[idx] = 1;
            completed++;
            current_time = p[idx].completion_time;
            prev = current_time;
        }
        else {
            // No process available, CPU idle
            current_time++;
        }
    }

    // Calculate averages
    avg_turnaround_time = (float) total_turnaround_time / n;
    avg_waiting_time = (float) total_waiting_time / n;
    cpu_utilisation = ((float)(current_time - total_idle_time) / current_time) * 100;

    // Print results
    printf("\n\nProcess Table:\n");
    printf("#P\tAT\tBT\tPRI\tST\tCT\tTAT\tWT\n\n");

    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].priority,
               p[i].start_time,
               p[i].completion_time,
               p[i].turnaround_time,
               p[i].waiting_time);
    }

    printf("\nStatistics:\n");
    printf("Average Turnaround Time = %.2f\n", avg_turnaround_time);
    printf("Average Waiting Time = %.2f\n", avg_waiting_time);
    printf("CPU Utilization = %.2f%%\n", cpu_utilisation);

    return 0;
}