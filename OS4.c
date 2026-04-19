#include <stdio.h>

int main() {

    int n, tq;

    printf("Enter number of processes and time quantum: ");
    scanf("%d %d",&n,&tq);

    int at[n], bt[n];

    printf("Enter arrival time and burst time:\n");
    for(int i=0;i<n;i++)
        scanf("%d %d",&at[i],&bt[i]);

    int rt[n], ct[n], tat[n], wt[n];

    // remaining time initially equals burst time
    for(int i=0;i<n;i++)
        rt[i] = bt[i];

    int queue[100];
    int front = 0, rear = 0;

    int visited[n];     // track process that haven't been into queue yet, 
    for(int i=0;i<n;i++)
        visited[i] = 0;

    int time = 0;
    int completed = 0;

    while(completed < n) {

        // insert processes that have arrived
        for(int i=0;i<n;i++) {
            if(at[i] <= time && visited[i] == 0) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        // if ready queue empty → CPU idle
        if(front == rear) {
            time++;
            continue;
        }

        // pick process from queue
        int i = queue[front++];

        if(rt[i] > tq) {

            time += tq;
            rt[i] -= tq;

        } else {

            time += rt[i];
            rt[i] = 0;

            ct[i] = time;
            completed++;
        }

        // add newly arrived processes during execution
        for(int j=0;j<n;j++) {
            if(at[j] <= time && visited[j] == 0) {
                queue[rear++] = j;
                visited[j] = 1;
            }
        }

        // if process not finished, push back to queue
        if(rt[i] > 0)
            queue[rear++] = i;
    }

    printf("\nP\tAT\tBT\tCT\tTAT\tWT\n");

    for(int i=0;i<n;i++) {

        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];

        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               i+1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    return 0;
}