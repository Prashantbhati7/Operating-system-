#include <stdio.h>

struct Process {          // making structure of the process by including the all required parameters 
    int pid;
    int arrival;
    int burst;
    int completion;
    int tat;
    int wt;
};

int main() {
    int n = 5;
    struct Process p[5] = {
        {1, 12, 6}, 
        {2, 2, 3},
        {3, 25, 4},
        {4, 20, 2},
        {5, 7, 4}
    };

    // Step 1: Sort processes by arrival time (FCFS rule)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival > p[j].arrival) {
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    int currentTime = 0;

    //  Calculate times
    for (int i = 0; i < n; i++) {
        // CPU idle case
        if (currentTime < p[i].arrival)      // idle time i.e when the next process arrival time is greater than the current time 
                                             // for exam the time between p5's completion time and p2's arrival time (i.e 7-5)

            currentTime = p[i].arrival;      // increasing the current time to the next process 's arrival time 

        currentTime += p[i].burst;           // simulating the execution of the process by increasing the time with value of burst time 
        p[i].completion = currentTime;       // marking storing process's completion time 

        p[i].tat = p[i].completion - p[i].arrival;     // calculating the tat of the process as completion time - arrival time 
        p[i].wt = p[i].tat - p[i].burst;        //  calculating waiting time as turn around time - burst time (total time of process - time in cpu )
    }

    //   Display result
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].completion,
               p[i].tat,
               p[i].wt);
    }

    return 0;
}


