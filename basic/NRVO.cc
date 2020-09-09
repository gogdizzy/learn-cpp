//
// Created by 关鑫 on 2020/7/1.
//

#include <stdio.h>
#include <utility>

struct A
{
    A( const char *p ) { this->p = p; printf( "  %s constructor\n", p ); }
    A( const A &a ) { this->p = "nil"; printf( "  %s->%s copy constructor\n", a.p, p ); }
    A( A &&a ) { this->p = "nil"; printf( "  %s->%s move constructor\n", a.p, p ); }
    ~A() { printf( "  %s destructor\n", p ); }
    A& operator=( const A &a ) { printf( "  %s->%s copy assign\n", a.p, p ); return *this; }
    A& operator=( A &&a ) { printf( "  %s->%s move assign\n", a.p, p ); return *this; }

    const char* p;
    int x;
};

A CouldURVO( int x ) {
    if( x == 0 ) return A( "zero" );
    return A( "nonZero" );
}

A CouldNRVO( int x ) {
    A a( "local1" );
    if( x == 0 ) return a;
    a.x = x;
    return a;
}

A CouldNotNRVO_ByMove( int x ) {
    A a( "local2" );
    a.x = x;
    return std::move( a );
}

A CouldNotNRVO_ByDiffRet( int x ) {
    A a( "local3" );
    A b( "local4" );
    return x ? a : b;
}

int main()
{
    printf( "CouldURVO:\n" );
    {
        A a = CouldURVO( 0 );
        a = CouldURVO( 5 );
    }
    printf( "\n\n" );

    printf( "CouldNRVO:\n" );
    {
        A a = CouldNRVO( 0 );
        a = CouldNRVO( 5 );
    }
    printf( "\n\n" );

    printf( "CouldNotNRVO_ByMove:\n" );
    {
        A a = CouldNotNRVO_ByMove( 0 );
        a = CouldNotNRVO_ByMove( 5 );
    }
    printf( "\n\n" );

    printf( "CouldNotNRVO_ByDiffRet:\n" );
    {
        A a = CouldNotNRVO_ByDiffRet( 0 );
        a = CouldNotNRVO_ByDiffRet( 5 );
    }
    printf( "\n\n" );

    return 0;
}