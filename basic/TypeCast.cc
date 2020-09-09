//
// Created by 关鑫 on 2020/9/9.
//

#include <cstdio>

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
    int x = int( T() ); // 显示转换
    printf( "explicit x = %d\n", x );
    printf( "T() = %d\n", T() ); // T()是一个临时变量，会取这个临时变量地址，然后按int*解释，内容未定
    test0( T() ); // 做了隐式转换
}

}

int main() {
    TestCast::run();
    return 0;
}