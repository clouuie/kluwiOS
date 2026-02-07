#include <iostream>
using namespace std;

struct process{
    int pid;                // ADDED
    int priority;
    int burst_time;
    int tt_time;
    int total_time = 0;
};

struct queues{
    int priority_start;
    int priority_end;
    int total_time = 0;
    int length = 0;
    process *p;
    bool executed = false;
};

bool notComplete(queues q[]){
    bool a = false;
    for(int i = 0; i < 3; i++){
        int countInc = 0;
        for(int j = 0; j < q[i].length; j++){
            if(q[i].p[j].burst_time != 0){
                a = true;
            } else {
                countInc++;
            }
        }
        if(countInc == q[i].length){
            q[i].executed = true;   // NOTE: copy issue fixed via wrapper
        }
    }
    return a;
}

// ADDED: fixes pass-by-value problem
void updateExecutionStatus(queues q[]){
    for(int i = 0; i < 3; i++){
        int done = 0;
        for(int j = 0; j < q[i].length; j++){
            if(q[i].p[j].burst_time == 0)
                done++;
        }
        if(done == q[i].length)
            q[i].executed = true;
    }
}

void sort_ps(queues q){
    for(int i = 1; i < q.length; i++){
        for(int j = 0; j < q.length - 1; j++){
            if(q.p[j].priority < q.p[j + 1].priority){
                process temp = q.p[j + 1];
                q.p[j + 1] = q.p[j];
                q.p[j] = temp;
            }
        }
    }
}

void checkCompleteTimer(queues q[]){
    notComplete(q);
    for(int i = 0; i < 3; i++){
        if(q[i].executed == false){
            for(int j = 0; j < q[i].length; j++){
                if(q[i].p[j].burst_time != 0){
                    q[i].p[j].total_time += 1;
                }
            }
            q[i].total_time += 1;
        }
    }
}

int main(){

    queues q[3];
    q[0].priority_start = 7; q[0].priority_end = 9;
    q[1].priority_start = 4; q[1].priority_end = 6;
    q[2].priority_start = 1; q[2].priority_end = 3;

    int no_of_processes, priority_of_process, burst_time_of_process;
    cout << "Enter the number of processes\n";
    cin >> no_of_processes;

    process p1[no_of_processes];

    for(int i = 0; i < no_of_processes; i++){
        cout << "Enter the priority of the process\n";
        cin >> priority_of_process;
        cout << "Enter the burst time of the process\n";
        cin >> burst_time_of_process;

        p1[i].pid = i + 1;                     // ADDED
        p1[i].priority = priority_of_process;
        p1[i].burst_time = burst_time_of_process;
        p1[i].tt_time = burst_time_of_process;

        for(int j = 0; j < 3; j++){
            if(q[j].priority_start <= priority_of_process &&
               priority_of_process <= q[j].priority_end){
                q[j].length++;
            }
        }
    }

    for(int i = 0; i < 3; i++){
        q[i].p = new process[q[i].length];
    }

    int a = 0, b = 0, c = 0;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < no_of_processes; j++){
            if(q[i].priority_start <= p1[j].priority &&
               p1[j].priority <= q[i].priority_end){
                if(i == 0) q[i].p[a++] = p1[j];
                else if(i == 1) q[i].p[b++] = p1[j];
                else q[i].p[c++] = p1[j];
            }
        }
    }

    cout << "\nQueue contents:\n";
    for(int i = 0; i < 3; i++){
        cout << "Queue " << i + 1 << " : ";
        for(int j = 0; j < q[i].length; j++){
            cout << "P" << q[i].p[j].pid << " -> ";
        }
        cout << "NULL\n";
    }

    int timer = 0, l = -1;
    int rr_timer = 4;
    int counter = 0, counterps = 0, counterfcfs = 0;

    while(notComplete(q)){
        if(timer == 10) timer = 0;

        l = (l + 1) % 3;

        if(q[l].executed){
            cout << "Queue " << l + 1 << " completed\n";
            continue;
        }

        if(l == 0){
            counter = 0;   // ADDED
            cout << "Queue 1 in hand\n";

            for(int i = 0; i < q[l].length && timer != 10; i++){
                if(q[l].p[i].burst_time == 0){
                    counter++;
                    continue;
                }

                rr_timer = 4;
                while(rr_timer > 0 && q[l].p[i].burst_time != 0 && timer != 10){
                    cout << "Executing Queue 1, P" << q[l].p[i].pid << "\n";
                    q[l].p[i].burst_time--;
                    checkCompleteTimer(q);
                    updateExecutionStatus(q);
                    rr_timer--;
                    timer++;
                }
            }
        }

        else if(l == 1){
            counterps = 0;  // ADDED
            cout << "Queue 2 in hand\n";
            sort_ps(*(q + l));   // FIXED CALL

            for(int i = 0; i < q[l].length && timer != 10; i++){
                if(q[l].p[i].burst_time == 0){
                    counterps++;
                    continue;
                }

                while(q[l].p[i].burst_time != 0 && timer != 10){
                    cout << "Executing Queue 2, P" << q[l].p[i].pid << "\n";
                    q[l].p[i].burst_time--;
                    checkCompleteTimer(q);
                    updateExecutionStatus(q);
                    timer++;
                }
            }
        }

        else{
            counterfcfs = 0;  // ADDED
            cout << "Queue 3 in hand\n";

            for(int i = 0; i < q[l].length && timer != 10; i++){
                if(q[l].p[i].burst_time == 0){
                    counterfcfs++;
                    continue;
                }

                while(q[l].p[i].burst_time != 0 && timer != 10){
                    cout << "Executing Queue 3, P" << q[l].p[i].pid << "\n";
                    q[l].p[i].burst_time--;
                    checkCompleteTimer(q);
                    updateExecutionStatus(q);
                    timer++;
                }
            }
        }
    }

    cout << "\nFinal Statistics:\n";
    int sum_tt = 0, sum_wt = 0;

    cout << "PID | Turnaround Time | Waiting Time\n";
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < q[i].length; j++){
            int wt = q[i].p[j].total_time - q[i].p[j].tt_time;
            cout << "P" << q[i].p[j].pid << "  | "
                 << q[i].p[j].total_time << "              | "
                 << wt << "\n";
            sum_tt += q[i].p[j].total_time;
            sum_wt += wt;
        }
    }

    cout << "\nAverage Turnaround Time: "
         << (float)sum_tt / no_of_processes << endl;
    cout << "Average Waiting Time: "
         << (float)sum_wt / no_of_processes << endl;

    return 0;
}

