//
// Created by 关鑫 on 2020/7/30.
//

#include <iostream>
#include <tuple>
#include <type_traits>


template<typename T> struct is_tuple : std::false_type {};
template<typename... Ts> struct is_tuple<std::tuple<Ts...>> : std::true_type {};
struct WildcardType {
    template<typename T>
    operator T();
};

// 判断T是否有N个成员变量, 编译期实现, 递归情形
template<typename T, size_t N, typename... U>
struct HasField {
    constexpr operator bool () {
        if constexpr (is_tuple<T>()) {
            return N == std::tuple_size<T>::value;
        } else {
            return HasField<T, N-1, WildcardType, U...>();
        }
    }
};

// 判断T是否有N个成员变量, 递归终止情形
template<typename T, typename... U>
struct HasField<T, 0, U...> {
    constexpr operator bool () {
        return std::is_constructible<T, U...>{};
    }
};

struct A {
    int a;
    float b;
    double c;
};


int main()
{
    std::cout << HasField< A, 3 >() << '\n';
    return 0;
}