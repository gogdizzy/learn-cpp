//
// Created by 关鑫 on 2020/9/9.
//

#include <iostream>
#include <omp.h>

namespace TestMultiOutput {

void run() {
    std::cout << "BEGIN" << std::endl;
#pragma omp parallel for
    for (char i = 'a'; i <= 'z'; i++)
        std::cout << i << std::endl;
    std::cout << "END" << std::endl;
}

}

namespace TestThreadInfo {

void run() {
    int nthreads, tid;

    /* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel private(nthreads, tid)
    {

        /* Obtain thread number */
        tid = omp_get_thread_num();
        printf("Hello World from thread = %d\n", tid);

        /* Only master thread does this */
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }

    }  /* All threads join master thread and disband */
}
}

int main() {
    TestMultiOutput::run();
    TestThreadInfo::run();

    return 0;
}