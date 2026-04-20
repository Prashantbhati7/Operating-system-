#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

bool flag[2] = {false, false};
int turn;
int shared = 0;

void *process0(void *arg) {
    for (int i = 0; i < 5; i++) {
        flag[0] = true;
        turn = 1;

        while (flag[1] && turn == 1);

        // Critical Section
        printf("Process 0 in Critical Section\n");
        shared++;
        printf("Shared value: %d\n", shared);
        sleep(1);

        flag[0] = false;

        // Remainder Section
        printf("Process 0 in Remainder Section\n");
        sleep(1);
    }
    return NULL;
}

void *process1(void *arg) {
    for (int i = 0; i < 5; i++) {
        flag[1] = true;
        turn = 0;

        while (flag[0] && turn == 0);

        // Critical Section
        printf("Process 1 in Critical Section\n");
        shared++;
        printf("Shared value: %d\n", shared);
        sleep(1);

        flag[1] = false;

        // Remainder Section
        printf("Process 1 in Remainder Section\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t t0, t1;

    pthread_create(&t0, NULL, process0, NULL);
    pthread_create(&t1, NULL, process1, NULL);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    printf("Final Shared Value: %d\n", shared);
    return 0;
}