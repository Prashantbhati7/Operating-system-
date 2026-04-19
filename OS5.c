#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int N;           // Buffer size entered by user
int *buffer;     // Dynamically allocated buffer
int in = 0, out = 0;

sem_t empty, full;
pthread_mutex_t mutex;

// Producer function
void* producer(void* arg) {
    int item;
    for(int i = 0; i < 10; i++) {
        item = rand() % 100;

        sem_wait(&empty);              // wait if buffer full (empty is zero) otherwiser decrease empty by 1 
        pthread_mutex_lock(&mutex);    // enter critical section if not entered otherwise put thread to sleep 

        buffer[in] = item;             // put item in buffer
        printf("Produced: %d at index %d\n", item, in);
        in = (in + 1) % N;   // increment in

        pthread_mutex_unlock(&mutex);  // exit critical section 
        sem_post(&full);               // signal item available , ping! wake up consumer thread if was sleeping waiting for a full slot 

        sleep(1);
    }
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    int item;
    for(int i = 0; i < 10; i++) {

        sem_wait(&full);               // wait if buffer empty (full is zero) otherwise increase the decrease full by 1 
        pthread_mutex_lock(&mutex);    // enter critical section if not entered otherwise put consumer thread to sleep 

        item = buffer[out];            // take item from buffer
        printf("Consumed: %d from index %d\n", item, out);
        out = (out + 1) % N; // increment out

        pthread_mutex_unlock(&mutex);  // exit critical section
        sem_post(&empty);              // signal space available , by incrementing empty by 1 , ping! wake up producer thread 

        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    printf("Enter buffer size (N): ");
    scanf("%d", &N);

    buffer = (int*)malloc(N * sizeof(int)); // allocate buffer dynamically

    sem_init(&empty, 0, N); // initially all N slots are empty
    sem_init(&full, 0, 0);  // initially no items
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    free(buffer); // free dynamically allocated buffer

    return 0;
}