//
// Created by 关鑫 on 2019/10/17.
//

#include <thread>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int sig = 0;

void handler(int sig) {
    void *array[32];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 32);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);

    exit(1);
}

void baz() {
    int *foo = (int*)-1;        // make a bad pointer
    if(sig == SIGSEGV) printf("%d\n", *foo);       // causes SIGSEGV
    abort();                    // causes SIGABRT
}

void bar() { baz(); }
void foo() { bar(); }

int main(int argc, char **argv) {

    if( argc > 1 ) sig = atoi( argv[1] );

    signal(SIGSEGV, handler);   // install our handler
    signal(SIGABRT, handler);   // install our handler

    std::thread t(foo);

    t.join();

    return 0;
}