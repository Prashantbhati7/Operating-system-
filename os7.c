#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];


int count = 0;
int in = 0, out = 0;

// Mutex ensures that only one thread accesses the buffer at a time (mutually exclusive)
pthread_mutex_t mutex;

// 'not_full' tells the producer when there is space to add items
// 'not_empty' tells the consumer when there is an item to take
pthread_cond_t not_full, not_empty;

void* producer(void* arg) {
    int item;

    while (1) {
        // Generate a random piece of data (0 to 99)
        item = rand() % 100;

        // Before touching the buffer, we MUST lock the mutex
        pthread_mutex_lock(&mutex);

        // If the buffer is full, the producer has to wait.
        // We use a 'while' loop in case of "spurious wakeups".
        while (count == BUFFER_SIZE) {
            printf("Producer: Buffer is full! Sleeping until there's space...\n");
            // This waits for 'not_full' and automatically releases/re-acquires the mutex
            pthread_cond_wait(&not_full, &mutex);
        }

        // Put the item in the buffer at the current 'in' position
        buffer[in] = item;
        printf("Produced: %d at index %d\n", item, in);
        
        // Move the 'in' pointer forward (circularly)
        in = (in + 1) % BUFFER_SIZE;
        count++;

        // Since we just added an item, the buffer is definitely NOT empty.
        // Signal the consumer just in case they were waiting.
        pthread_cond_signal(&not_empty);

        // Release the lock so the consumer (or someone else) can access the buffer
        pthread_mutex_unlock(&mutex);

        // Slow down a bit to make it easier to see what's happening
        sleep(1);
    }
}

void* consumer(void* arg) {
    int item;

    while (1) {
        // Always lock the mutex before checking or touching shared data
        pthread_mutex_lock(&mutex);

        // If the buffer is empty, there's nothing to eat. Wait!
        while (count == 0) {
            printf("Consumer: Buffer is empty! Sleeping until producer adds something...\n");
            // This waits for 'not_empty' and pauses the thread efficiently
            pthread_cond_wait(&not_empty, &mutex);
        }

        // Take the item from the buffer at the current 'out' position
        item = buffer[out];
        printf("Consumed: %d from index %d\n", item, out);
        
        // Move the 'out' pointer forward (circularly)
        out = (out + 1) % BUFFER_SIZE;
        count--;

        // Since we just removed an item, the buffer is definitely NOT full anymore.
        // Signal the producer just in case they were waiting for space.
        pthread_cond_signal(&not_full);

        // Don't forget to unlock the mutex!
        pthread_mutex_unlock(&mutex);

        // Slow down a bit
        sleep(1);
    }
}

int main() {
    pthread_t p, c;

    // Initialize our synchronization tools
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    // Create the two threads: one for producing and one for consuming
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    // Wait for the threads to finish (though these run forever in this example)
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    //Clean up resources (this line won't actually be reached here)
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}