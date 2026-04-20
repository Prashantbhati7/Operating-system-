#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5  // number of readers

sem_t mutex, wrt;
int readCount = 0;
int sharedData = 0;

void *reader(void *arg) {
    int id = *(int *)arg;

    // Entry section
    sem_wait(&mutex);
    readCount++;
    if (readCount == 1) {
        sem_wait(&wrt); // first reader blocks writer
    }
    sem_post(&mutex);

    // Critical section
    printf("Reader %d is reading data = %d\n", id, sharedData);
    sleep(1);

    // Exit section
    sem_wait(&mutex);
    readCount--;
    if (readCount == 0) {
        sem_post(&wrt); // last reader releases writer
    }
    sem_post(&mutex);

    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;

    sem_wait(&wrt); // exclusive access

    // Critical section
    sharedData++;
    printf("Writer %d is writing data = %d\n", id, sharedData);
    sleep(2);

    sem_post(&wrt);

    return NULL;
}

int main() {
    pthread_t r[N], w;
    int r_id[N], w_id = 1;

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    // Create writer
    pthread_create(&w, NULL, writer, &w_id);

    // Create readers
    for (int i = 0; i < N; i++) {
        r_id[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &r_id[i]);
    }

    // Join threads
    for (int i = 0; i < N; i++) {
        pthread_join(r[i], NULL);
    }
    pthread_join(w, NULL);

    sem_destroy(&mutex);
    sem_destroy(&wrt);
    
    return 0;
}