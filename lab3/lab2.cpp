#include "lab2.h"
#include <cstring>
#include <semaphore.h>

#define NUMBER_OF_THREADS 11

// thread identifiers
pthread_t tid[NUMBER_OF_THREADS];
// critical section lock
pthread_mutex_t lock;
// semaphores for sequential threads
sem_t semaphoreB, semaphoreC, semaphoreD, semaphoreG, semaphoreH, semaphoreK;

int err;

unsigned int lab2_thread_graph_id() {
    return 6;
}

const char* lab2_unsynchronized_threads() {
    return "deg";
}

const char* lab2_sequential_threads() {
    return "bcd";
}

void *thread_a(void *ptr); // 0
void *thread_b(void *ptr); // 1
void *thread_c(void *ptr); // 2
void *thread_d(void *ptr); // 3
void *thread_e(void *ptr); // 4
void *thread_g(void *ptr); // 5
void *thread_f(void *ptr); // 6
void *thread_h(void *ptr); // 7
void *thread_i(void *ptr); // 8
void *thread_k(void *ptr); // 9
void *thread_m(void *ptr); // 10

void *thread_a(void *ptr) {
    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "a" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    sem_post(&semaphoreB);
    err = pthread_create(&tid[1], NULL, thread_b, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
    err = pthread_create(&tid[2], NULL, thread_c, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
    err = pthread_create(&tid[3], NULL, thread_d, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
    
    return  ptr;
}

void *thread_b(void *ptr) {
    for (int i = 0; i < 3; ++i) {
        sem_wait(&semaphoreB);
        pthread_mutex_lock(&lock);
        std::cout << "b" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&semaphoreC);
    }

    return ptr;
}

void *thread_c(void *ptr) {
    for (int i = 0; i < 3; ++i) {
        sem_wait(&semaphoreC);
        pthread_mutex_lock(&lock);
        std::cout << "c" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&semaphoreD);
    }
    
    return ptr;
}


void *thread_d(void *ptr) {
    for (int i = 0; i < 3; ++i) {
        sem_wait(&semaphoreD);
        pthread_mutex_lock(&lock);
        std::cout << "d" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&semaphoreB);
    }
    
    err = pthread_create(&tid[4], NULL, thread_e, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
    err = pthread_create(&tid[5], NULL, thread_g, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "d" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
 
    // end of T3
    sem_post(&semaphoreG);

    return ptr;
}

void *thread_e(void *ptr) {
    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "e" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return ptr;
}

void *thread_g(void *ptr) {
    for (int i = 0; i < 3; ++i) {        
        pthread_mutex_lock(&lock);
        std::cout << "g" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    pthread_join(tid[4], NULL);
    sem_wait(&semaphoreG);

    err = pthread_create(&tid[6], NULL, thread_f, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
    err = pthread_create(&tid[7], NULL, thread_h, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

    for (int i = 0; i < 3; ++i) {        
        pthread_mutex_lock(&lock);
        std::cout << "g" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    sem_post(&semaphoreH);

    return ptr;
}

void *thread_f(void *ptr) {
    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "f" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return ptr;
}

void *thread_h(void *ptr) {
    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "h" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    pthread_join(tid[6], NULL);
    sem_wait(&semaphoreH);

    err = pthread_create(&tid[8], NULL, thread_i, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
    err = pthread_create(&tid[9], NULL, thread_k, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

   for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "h" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
 
   sem_post(&semaphoreK);

    return ptr;
}

void *thread_i(void *ptr) {
    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "i" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return ptr;
}

void *thread_k(void *ptr) {
    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "k" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    pthread_join(tid[8], NULL);
    sem_wait(&semaphoreK);

    err = pthread_create(&tid[10], NULL, thread_m, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "k" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }

    return ptr;
}

void *thread_m(void *ptr) {
    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "m" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return ptr;
}

int lab2_init() {
    // initilize mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        std::cerr << "Mutex init failed" << std::endl;
        return 1;
    }
    // initialize semaphores
    // THIS CODE WILL NOT RUN ON MacOS!
    // MacOS doesn't support unnamed semaphores, so named semaphores should be used instead
    if ( sem_init(&semaphoreB, 0, 0) != 0 ) {
        std::cerr << "Semaphore #1 init failed" << std::endl;
        return 1;
    }

    if ( sem_init(&semaphoreC, 0, 0) != 0 ) {
        std::cerr << "Semaphore #2 init failed" << std::endl;
        return 1;
    }

    if ( sem_init(&semaphoreD, 0, 0) != 0 ) {
        std::cerr << "Semaphore #3 init failed" << std::endl;
        return 1;
    }

    if ( sem_init(&semaphoreH, 0, 0) != 0 ) {
        std::cerr << "Semaphore #4 init failed" << std::endl;
        return 1;
    }

    if ( sem_init(&semaphoreK, 0, 0) != 0 ) {
        std::cerr << "Semaphore #5 init failed" << std::endl;
        return 1;
    }
   
    // start the first thread
    err = pthread_create(&tid[0], NULL, thread_a, NULL);
    if (err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

    // ... and wait for it to finish
    pthread_join(tid[0], NULL);

    // wait for other thread to finish
    pthread_join(tid[3], NULL);
    pthread_join(tid[5], NULL);
    pthread_join(tid[7], NULL);
    pthread_join(tid[9], NULL);
    pthread_join(tid[10], NULL);
 
    // free resources
    pthread_mutex_destroy(&lock);
    sem_destroy(&semaphoreB);
    sem_destroy(&semaphoreC);
    sem_destroy(&semaphoreD);
    sem_destroy(&semaphoreG);
    sem_destroy(&semaphoreH);
    sem_destroy(&semaphoreK);

    std::cout << std::endl;
    // success
    return 0;
}
