//
// Created by 关鑫 on 2019/10/9.
//


#include <cstdio>
#include <iostream>

using std::cout;
using std::endl;

namespace TestCast {

class T {
public:
    operator int () {
        return 10;
    }
};

void test0( int x ) {
    printf( "implicit x = %d\n", x );
}

void run() {
    int x = int( T() );
    printf( "explicit x = %d\n", x );
    printf( "T() = %d\n", T() );
    test0( T() );
}

}

namespace TestNewOverride {

class Test {
public:
    Test() {
        cout << "Test()" << endl;
    }

    // 无论是否加static，都会编译成静态版本
    static void *operator new( size_t size ) {
        void *ret = malloc( sizeof( int ) * size );
        cout << "normal new" << endl;
        return ret;
    }

    static void *operator new( size_t size, void *loc ) {
        cout << "placement new" << endl;
        return loc;
    }

};

void run() {
    Test* t = new Test();
    Test* t1 = new((void*)t)Test();
    Test t2;
    int a = 3;
    int* p = new((void*)&a)int(10);
    cout << "a = " << a << endl;
}

}

namespace TestRvalueRef {

void run() {

}

}

int main() {
    TestCast::run();
    TestNewOverride::run();
    TestRvalueRef::run();
    return 0;
}
