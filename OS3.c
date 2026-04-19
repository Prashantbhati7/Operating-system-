#include <stdio.h>

int main() {
    int n ;
    printf("enter no of processes");
    scanf("%d",&n);


    int pid[n],at[n],bt[n] ;
    for (int i = 0 ;i < n ;i++){
        printf("enter the process id , arrival time and burst time \n");
        scanf("%d %d %d",&pid[i],&at[i],&bt[i]);
    }

    int ct[n], tat[n], wt[n];
    int completed[n];
    for (int i = 0; i < n; i++) {
        completed[i] = 0;
    }

    int time = 0, completed_count = 0;
    int min_index;
    
    while (completed_count < n) {
        int min_bt = 100000;

        // Find shortest job among arrived processes
        for (int i = 0; i < n; i++) {
            if (at[i] <= time && completed[i] == 0) {
                if (bt[i] < min_bt) {
                    min_bt = bt[i];
                    min_index = i;
                }
            }
        }

        // If no process has arrived yet → CPU idle
        if (min_bt == 100000) {
            time++;
        }
        else {
            ct[min_index] = time + bt[min_index];
            time = ct[min_index];

            tat[min_index] = ct[min_index] - at[min_index];
            wt[min_index] = tat[min_index] - bt[min_index];

            completed[min_index] = 1;
            completed_count++;
        }
    }

    float total_tat = 0, total_wt = 0;

    printf("\nP\tAT\tBT\tCT\tTAT\tWT\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               pid[i], at[i], bt[i], ct[i], tat[i], wt[i]);

        total_tat += tat[i];
        total_wt += wt[i];
    }

    printf("\nAverage Turnaround Time = %.2f", total_tat / n);
    printf("\nAverage Waiting Time = %.2f", total_wt / n);
    printf("\nSchedule Length = %d\n", time);

    return 0;
}