#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Apple's official interpose macro structure
#define DYLD_INTERPOSE(_replacement,_replacee) \
   __attribute__((used)) static struct{ const void* replacement; const void* replacee; } _interpose_##_replacee \
            __attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacement, (const void*)(unsigned long)&_replacee };

void opposable_exit(int status) {
    if (!pthread_main_np()) {
        int *result = malloc(sizeof(status));
        *result = status;
        pthread_exit((void *)(intptr_t)result);
    } else
        exit(status);
}

void _opposable_exit(int status) {
    if (!pthread_main_np()) {
        int *result = malloc(sizeof(status));
        *result = status;
        pthread_exit((void *)(intptr_t)result);
    } else
        _exit(status);
}

// Redirect dynamic symbol lookups for exit and _exit
DYLD_INTERPOSE(opposable_exit, exit)
DYLD_INTERPOSE(_opposable_exit, _exit)
