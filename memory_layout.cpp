//
// Created by 关鑫 on 2019/10/9.
//

#include <stdio.h>

namespace TestVptr {

class A {
public:
    virtual ~A() {}
};

class D : public A {
public:
    long d;
};

class B: public A {
public:
    D data[3];
    void print() {
        printf( "B start at %p\nB::data start at %p\n", this, data );
    }
};

void run() {
    B b;
    b.print();
    printf( "sizeof A: %zd\n", sizeof(A) );
    printf( "sizeof B: %zd\n", sizeof(B) );
    printf( "sizeof D: %zd\n", sizeof(D) );
}

}

namespace TestInherit {

class A {
public:
    int a;
};

class B: public A {};

class C: public A {};

class D: public B, public C {
public:
    void f() {
        B::a = 1;
        C::a = 2;
    }
};

void run() {
    printf( "Diamond Inherit:\n" );
    printf( "   A\n" );
    printf( "  / \\\n" );
    printf( " B   C\n" );
    printf( "  \\ /\n" );
    printf( "   D\n" );
    printf( "sizeof(A) = %zd\n", sizeof(A) );
    printf( "sizeof(B) = %zd\n", sizeof(B) );
    printf( "sizeof(C) = %zd\n", sizeof(C) );
    printf( "sizeof(D) = %zd\n", sizeof(D) );
    D d;
    d.f();
    // A* pba = static_cast<A*>( d ); error
    A* pba = static_cast<A*>( static_cast<B*>(&d) );
    A* pca = static_cast<A*>( static_cast<C*>(&d) );
}

}



int main() {
    TestVptr::run();
    TestInherit::run();
    return 0;
}