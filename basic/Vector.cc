//
// Created by 关鑫 on 2020/9/9.
//

#include <iostream>
#include <vector>

namespace TestCopyAndCopybackward {

using std::cout;

void printVector( std::vector< int > &v ) {
    for( auto x : v ) {
        cout << x << ' ';
    }
    cout << '\n';
}

void run() {
    std::vector< int > numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::vector< int > n2;

    cout << "Vector Data:\n";
    printVector( numbers );

    n2 = numbers;
    cout << "it + copy:\n";
    std::copy( begin( n2 ), begin( n2 ) + 5, begin( n2 ) + 3 );
    printVector( n2 );

    n2 = numbers;
    cout << "it + copy_backward:\n";
    std::copy_backward( begin( n2 ), begin( n2 ) + 5, begin( n2 ) + 8 );
    printVector( n2 );

    n2 = numbers;
    cout << "reverse_it + copy:\n";
    std::copy( rbegin( n2 ), rbegin( n2 ) + 5, rbegin( n2 ) + 3 );
    printVector( n2 );

    n2 = numbers;
    cout << "reverse_it + copy_backward:\n";
    std::copy_backward( rbegin( n2 ), rbegin( n2 ) + 5, rbegin( n2 ) + 8 );
    printVector( n2 );
}

}

int main() {
    TestCopyAndCopybackward::run();
    return 0;
}

