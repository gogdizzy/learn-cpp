//
// Created by 关鑫 on 2020/9/9.
//

#include <iostream>

namespace TestNewOverride {

using std::cout;
using std::endl;

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

int main() {
    TestNewOverride::run();

    return 0;
}