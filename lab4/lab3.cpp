#include "lab3.h"
#include <cstdint>
#include <windows.h>
#include <string>

const int THREADS_COUNT = 12;
const int INVOCATION_COUNT = 3;
const int SEMAPHORES_COUNT = 3;
const int A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, H = 7, I = 8, K = 9, M = 10, N = 11;
const int semaphore1 = 12, semaphore2 = 13, semaphore3 = 14;

HANDLE threads[THREADS_COUNT];
HANDLE semaphore[THREADS_COUNT + 4];

void post_semaphore(int semaphore_name, int permissions_count) {
    if (!ReleaseSemaphore(semaphore[semaphore_name], permissions_count, NULL)) {
        printf("ReleaseSemaphore %d error: %d\n", semaphore_name, GetLastError());
    }
}

void run_thread_mutex(char thread_name) {
    for (int i = 0; i < INVOCATION_COUNT; ++i) {
        WaitForSingleObject(semaphore[N], INFINITE);
        std::cout << thread_name << std::flush;
        post_semaphore(N, 1);
        computation();
    }
}

void run_thread_semaphore(char thread_name, int block_sem_name, int unblock_sem_name) {
    for (int i = 0; i < INVOCATION_COUNT; ++i) {
        WaitForSingleObject(semaphore[block_sem_name], INFINITE);
        WaitForSingleObject(semaphore[N], INFINITE);
        std::cout << thread_name << std::flush;
        post_semaphore(N, 1);
        post_semaphore(unblock_sem_name, 1);
        computation();
    }
}

void process_thread_a() {
    run_thread_mutex('a');
    post_semaphore(A, 4);
}

void process_thread_b() {
    run_thread_mutex('b');
    post_semaphore(B, 3);

    WaitForSingleObject(semaphore[A], INFINITE);
    run_thread_mutex('b');
    WaitForSingleObject(semaphore[E], INFINITE);
    WaitForSingleObject(semaphore[D], INFINITE);
    WaitForSingleObject(semaphore[C], INFINITE);
    post_semaphore(B, 1);
}

void process_thread_c() {
    WaitForSingleObject(semaphore[B], INFINITE);
    WaitForSingleObject(semaphore[A], INFINITE);
    run_thread_mutex('c');
    WaitForSingleObject(semaphore[E], INFINITE);
    WaitForSingleObject(semaphore[D], INFINITE);
    post_semaphore(C, 2);
}

void process_thread_d() {
    WaitForSingleObject(semaphore[B], INFINITE);
    WaitForSingleObject(semaphore[A], INFINITE);
    run_thread_mutex('d');
    WaitForSingleObject(semaphore[E], INFINITE);
    post_semaphore(D, 3);
}

void process_thread_e() {
    WaitForSingleObject(semaphore[B], INFINITE);
    WaitForSingleObject(semaphore[A], INFINITE);
    run_thread_mutex('e');
    post_semaphore(E, 3);

    WaitForSingleObject(semaphore[I], INFINITE);
    run_thread_mutex('e');
    post_semaphore(E, 4);
}

void process_thread_f() {
    WaitForSingleObject(semaphore[I], INFINITE);
    run_thread_mutex('f');
    WaitForSingleObject(semaphore[E], INFINITE);
    post_semaphore(F, 3);
}

void process_thread_g() {
    WaitForSingleObject(semaphore[I], INFINITE);
    run_thread_mutex('g');
    WaitForSingleObject(semaphore[E], INFINITE);
    WaitForSingleObject(semaphore[F], INFINITE);
    post_semaphore(G, 2);
}

void process_thread_h() {
    WaitForSingleObject(semaphore[I], INFINITE);
    run_thread_mutex('h');
    WaitForSingleObject(semaphore[E], INFINITE);
    WaitForSingleObject(semaphore[F], INFINITE);
    WaitForSingleObject(semaphore[G], INFINITE);
    post_semaphore(H, 1);
}

void process_thread_i() {
    WaitForSingleObject(semaphore[C], INFINITE);
    WaitForSingleObject(semaphore[B], INFINITE);
    post_semaphore(I, 4);
    run_thread_mutex('i');
    WaitForSingleObject(semaphore[E], INFINITE);
    WaitForSingleObject(semaphore[F], INFINITE);
    WaitForSingleObject(semaphore[G], INFINITE);
    WaitForSingleObject(semaphore[H], INFINITE);

    post_semaphore(semaphore1, 1);
    run_thread_semaphore('i', semaphore1, semaphore2);
    WaitForSingleObject(semaphore[M], INFINITE);
    WaitForSingleObject(semaphore[K], INFINITE);

    post_semaphore(I, 1);
}

void process_thread_k() {
    run_thread_semaphore('k', semaphore2, semaphore3);
    WaitForSingleObject(semaphore[M], INFINITE);
    post_semaphore(K, 2);
}

void process_thread_m() {
    run_thread_semaphore('m', semaphore3, semaphore1);
    post_semaphore(M, 2);
}

void process_thread_n() {
    WaitForSingleObject(semaphore[K], INFINITE);
    WaitForSingleObject(semaphore[I], INFINITE);
    run_thread_mutex('n');
}

DWORD WINAPI process_threads(LPVOID thread_name) {
    switch ((intptr_t)thread_name) {
        case A:
            process_thread_a();
            break;
        case B:
            process_thread_b();
            break;
        case C:
            process_thread_c();
            break;
        case D:
            process_thread_d();
            break;
        case E:
            process_thread_e();
            break;
        case F:
            process_thread_f();
            break;
        case G:
            process_thread_g();
            break;
        case H:
            process_thread_h();
            break;
        case I:
            process_thread_i();
            break;
        case K:
            process_thread_k();
            break;
        case M:
            process_thread_m();
            break;
        case N:
            process_thread_n();
            break;
    }
    return 0;
}

void create_thread(intptr_t thread_name) {
    DWORD new_thread_id;
    threads[thread_name] = CreateThread(NULL, 0, process_threads, (void *) thread_name, 0, &new_thread_id);
    if (threads[thread_name] == NULL) {
        std::cout << "Creation of thread " << thread_name << " failed" << std::endl;
    }
}

void create_semaphore(int semaphore_name) {
    semaphore[semaphore_name] = CreateSemaphore(NULL, 0, 12, NULL);
    if (semaphore[semaphore_name] == NULL) {
        std::cout << "Creation of semaphore " << semaphore_name << " failed" << std::endl;
    }
}

unsigned int lab3_thread_graph_id() {
    return 2;
}

const char *lab3_unsynchronized_threads() {
    return "bcde";
}

const char *lab3_sequential_threads() {
    return "ikm";
}

int lab3_init() {
    for (int i = THREADS_COUNT + SEMAPHORES_COUNT; i >= 0; i--) {
        create_semaphore(i);
    }
post_semaphore(N, 1);
    for (int i = THREADS_COUNT - 1; i >= 0; i--) {
        create_thread(i);
    }


    WaitForMultipleObjects(THREADS_COUNT, threads, TRUE, INFINITE);
    for (int i = 0; i < THREADS_COUNT; i++) {
        CloseHandle(threads[i]);
        CloseHandle(semaphore[i]);
    }

    return 0;
}
