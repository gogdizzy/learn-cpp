//
// Created by 关鑫 on 2019/10/12.
//

#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie( nullptr );
    std::cout.tie( nullptr );
    std::cout << "1"; // << std::flush;
    // std::flush( std::cout );
    // std::cerr << "2" << "\n";
    fprintf( stderr, "2\n" );
    return 0;
}
