//
// Created by 关鑫 on 2020/7/6.
//

#include <atomic>
#include <cstdio>

template< typename T >
struct Atomic {
    Atomic() { printf( "constructor\n" ); }
    Atomic(const T &x) { printf( "constructor 2\n" ); this->x = x; }
    operator T() { printf( "operator T\n" ); return x; }
    T x;
};

int main() {

    printf( "start\n" );

    std::atomic< int > a = 1;

    if( a == 1 ) {
        printf( "Equal!\n" );
    }

    Atomic<int> b(1);
    if( b == 1 ) {
        printf( "Equal!\n" );
    }

    return 0;
}