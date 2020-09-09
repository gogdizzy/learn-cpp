//
// Created by 关鑫 on 2020/9/9.
//
#include <thread>
#include <iostream>
#include <chrono>
#include <cassert>

namespace TestSync {

void run() {
    std::cout << "a\n"; // buffered, not sync with c streams
    std::printf( "b\n" );
    std::cout << "c\n";
    std::cout << "sync_with_stdio: false\n";
    std::ios::sync_with_stdio( false );
    std::cout << "a\n"; // buffered, not sync with c streams
    std::printf( "b\n" );
    std::cout << "c\n" << std::flush; // 如果不加flush，a和c都会处于buffer中
}

}

namespace TestTie {

void run() {
    std::ios_base::sync_with_stdio( true );
    std::printf( "sync_with_stdio: false\n" );
    assert(nullptr == std::cout.tie(nullptr)); // std::cout 默认没有绑定别的流，std::cout也提供了这个方法，可以看出这些out流是可以链式绑定的

    std::cout << "x\n";
    std::printf( "x buffered\n" );
    int dummy;
    std::cin >> dummy; // 因为 std::cin.tie 返回 &std::cout，所以遇到输入，会调用std::cout.flush()，since c++11
    std::printf( "x not buffered\n" );
    std::printf( "dummy = %d\n", dummy );

    std::cout << "x\n";
    std::printf( "x buffered\n" );
    auto ptr = std::cin.tie( nullptr ); // 与 &std::cout 指针解绑
    assert(ptr == &std::cout);
    std::cin >> dummy; // 不会调用std::cout.flush()
    std::printf( "x still buffered\n" );
    std::printf( "dummy = %d\n", dummy );
    std::cout << std::flush;

    std::cout << "x\n";
    std::printf( "x buffered\n" );
    std::cerr << "err output\n"; // 调用std::cout.flush()
    std::printf( "x not buffered\n" );
}

}

namespace TestTie2 {

void f() {
    std::cout << "Output from thread...";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "...thread calls flush()" << std::endl;
}

void run() {
    std::thread t1(f);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::clog << "This output from main is not tie()'d to cout\n";
    std::cerr << "This output is tie()'d to cout\n";
    t1.join();
}
}

int main()
{
    // !!! 这个示例在clion中直接运行，交错的cout和cerr会有问题，最好单独在terminal中测试
    TestSync::run();
    TestTie::run();
    TestTie2::run();
    return 0;
}


