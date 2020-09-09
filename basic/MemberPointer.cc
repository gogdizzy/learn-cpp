//
// Created by 关鑫 on 2020/9/9.
//

#include <cstdio>

namespace TestMemberFunctionPointer {
template<typename T>
void Test(void (T::*fun)(), T* pObj)
{
    (pObj->*fun)();
}

class A {
protected:
    virtual void f() {
        printf( "call A::f\n" );
    }
};

class M :public A {};

class B : public M {
public:
    void test() {
        Test(&B::f, static_cast<A*>(this));
        // f();
    }
};

void run() {
    B b;
    b.test();
}

}

int main() {
    TestMemberFunctionPointer::run();
    return 0;
}