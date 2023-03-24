//
// Created by 关鑫 on 2021/2/5.
//

#include <stdio.h>
#include <stdarg.h>

enum {
    AAA,
    BBB
};

enum class E : int8_t {
    AAA,
    BBB
};

void rrlog(int a, int b, int c, int d, const char* s1, const char* s2, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

struct A { int a; int b; };

int main() {
    printf( "%zd %zd\n", sizeof(AAA), sizeof(E) );
    rrlog( 1, 2, 3, 4, "test", "test",
            "%d %d %d %d %d %d %d %d "
            "%d %d %d %d %d %d %d %d "
            "%d %d %d %d %d %d %d %d "
            "%d %d %d %d %d %d %d %d\n",
            E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA,
            E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA,
            E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA,
            E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA, E::AAA );

    A a;
    const A &b = a;
    return 0;
}