#include "stdio.h"

#define MAX_CB 8

typedef struct {
    int interval;
    int counter;
    void (*function)(void);
} s_callback;

s_callback callbacks[MAX_CB];

int n_cb = 0;

void add_callback(int interval, int counter, void (*fct)(void)) {
    printf("add_callback\n");
    if (n_cb<MAX_CB) {
        callbacks[n_cb].interval = interval;
        callbacks[n_cb].counter = counter;
        callbacks[n_cb].function = fct;
        printf("added callback(interval=%d, counter=%d)\n", 
                callbacks[n_cb].interval, callbacks[n_cb].counter);
        n_cb++;
    }
}

void dump_callbacks() {
    int i; 
    for (i=0; i<n_cb; i++) {
        printf("callbacks[%d].interval=%d .counter=%d\n",
                i, callbacks[i].interval, callbacks[i].counter);
    }
}

void test1() {
    printf("test1\n");
}

void test2() {
    printf("test2\n");
}

int main() {
    add_callback(100, 0, test1);
    add_callback(100, 50, test2);

    dump_callbacks();
    return 0;
}
