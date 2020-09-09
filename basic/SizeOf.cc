//
// Created by 关鑫 on 2020/9/9.
//

#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <unordered_map>

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
//    A* pa = static_cast<A*>( d ); error
    A* pba = static_cast<A*>( static_cast<B*>(&d) );
    A* pca = static_cast<A*>( static_cast<C*>(&d) );
}

}

namespace TestCase1 {

struct A {
    void *next;
};

template< typename K, typename V >
struct B : public A {
    __gnu_cxx::__aligned_buffer< std::pair<const K, V> > v;
    // std::pair<const K, V> v;
};

template< typename K, typename V >
struct HashNode : public B< K, V > {
    // std::size_t hc;
};

struct SparseGridMap {
    std::unordered_map<int32_t, int64_t> m;
    bool b;
    int64_t def;
};

template< typename K >
struct Key {
    K first;
};

template< typename V >
struct Value {
    V second;
};

template< typename K, typename V >
struct iterator : public Key<K>, public Value<V> {};

struct FibKey {
    int32_t point;
    int64_t k1;
    int64_t k2;
    void *vertex;
};
struct FibKey2 {
    int32_t point;
    void *vertex;
    int64_t k1;
    int64_t k2;
};

void run() {

#define P(...) std::cout << #__VA_ARGS__ << " : " << sizeof(__VA_ARGS__) << '\n'

    P(size_t);
    P(void*);
    P(int32_t);
    P(int64_t);
    P(std::pair<const int32_t, int64_t>);
    P(iterator<int32_t, int64_t>);
    P(A);
    P(B<int32_t, int64_t>);
    P(HashNode<int32_t, int64_t>);
    P(std::unordered_map<int32_t, int64_t>);
    P(std::unordered_map<int32_t, int64_t>::iterator);

    P(SparseGridMap);
    P(iterator<int32_t, SparseGridMap>);
    P(std::pair<const int32_t, SparseGridMap>);
    P(HashNode<int32_t, SparseGridMap>);
    P(std::unordered_map<int32_t, SparseGridMap>);
    P(std::unordered_map<int32_t, SparseGridMap>::iterator);
    P(FibKey);
    P(FibKey2);

#undef P
}

}



int main() {
    TestVptr::run();
    TestInherit::run();
    TestCase1::run();
    return 0;
}