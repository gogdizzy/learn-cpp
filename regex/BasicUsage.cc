//
// Created by 关鑫 on 2020/9/9.
//

#include <iostream>
#include <iterator>
#include <regex>
#include <string>

namespace TestRegexReplace {

void run() {
    int a = 10;
    std::cout << std::regex_replace("123", std::regex("^"), "T") << '\n';
    std::cout << std::regex_replace("123", std::regex("$"), "T") << '\n';
    std::cout << std::regex_replace("123", std::regex("(.*)$", std::regex::extended), "$1T") << '\n';
}

}

void run() {
    std::string s1("123");
    std::regex pattern("(.*)$");
    std::string str("$1T");
    auto res = std::regex_replace(s1, pattern, str);
    std::cout << res << '\n';
}

int main() {
    int z = 10;
    // TestRegexReplace::run();
    run();

    return 0;
}