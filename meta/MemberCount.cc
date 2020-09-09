//
// Created by 关鑫 on 2020/7/30.
//

#include <iostream>

struct UniversalType {
    template<typename T>
    operator T() {}
};

template<typename T, auto ...Members>
consteval auto MemberCounter() {
    if constexpr (requires { T{ (Members, UniversalType{})... }; } == false)
        return sizeof...(Members) - 1;
    else
        return MemberCounter<T, Members..., 0>();
}

template<typename T>
consteval auto MemberCounter(auto ...Members) {
    if constexpr (requires { T{ Members... }; } == false)
    return sizeof...(Members) - 1;
    else
    return MemberCounter<T>(Members..., UniversalType{});
}

auto main()->int {
    using TestType = struct { int x; float y; char z; struct {} w;};
    std::cout << MemberCounter<TestType>() << std::endl;
}

