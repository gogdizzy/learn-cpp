//
// Created by 关鑫 on 2020/9/9.
//

#include <iostream>
#include <vector>
#include <deque>

namespace TestCopyAndCopybackward {

using std::cout;
typedef std::vector< int > MyContainer;

void printVector( MyContainer &v ) {
    for( auto x : v ) {
        cout << x << ' ';
    }
    cout << '\n';
}

void run() {
    // 讲道理，如果源和目的有overlap，那么我们应该用 （it or rit) + copy_backward 组合
    // 但是我们可以发现， it + copy 也是正确的，为什么没有被覆盖呢？
    // 因为为了优化，当发现数据是连续的，编译器可能调用memmove，而memmove是保证即使有overlap也是安全的
    // 如果使用deque，就能发现有覆盖现象，不过数字要稍多一些，跨过deque内部的block

    MyContainer numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    MyContainer n2;

    cout << "Vector Data:\n";
    printVector( numbers );

    n2 = numbers;
    cout << "it + copy:\n";
    std::copy( std::begin( n2 ), std::begin( n2 ) + 5, std::begin( n2 ) + 3 );
    printVector( n2 );

    n2 = numbers;
    cout << "it + copy_backward:\n";
    std::copy_backward( std::begin( n2 ), std::begin( n2 ) + 5, std::begin( n2 ) + 8 );
    printVector( n2 );

    n2 = numbers;
    cout << "reverse_it + copy:\n";
    std::copy( std::rbegin( n2 ), std::rbegin( n2 ) + 5, std::rbegin( n2 ) + 3 );
    printVector( n2 );

    n2 = numbers;
    cout << "reverse_it + copy_backward:\n";
    std::copy_backward( std::rbegin( n2 ), std::rbegin( n2 ) + 5, std::rbegin( n2 ) + 8 );
    printVector( n2 );
}

}

int main() {
    TestCopyAndCopybackward::run();
    return 0;
}

