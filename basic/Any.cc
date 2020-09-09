//
// Created by 关鑫 on 2020/7/31.
//

#include <iostream>
#include <any>

template < typename T, std::enable_if_t<std::is_integral<T>::value, bool> = true>
class A {};


struct in_place_t {
    explicit in_place_t() = default;
};

inline constexpr in_place_t in_place{};

template<typename _Tp> struct in_place_type_t
{
    explicit in_place_type_t() = default;
};

template<typename _Tp>
inline constexpr in_place_type_t<_Tp> in_place_type{};

template<typename _Up>
static void
_S_create(_Up&& __value)
{
    std::cout << "1\n";
}

template<typename... _Args>
static void
_S_create(_Args&&... __args)
{
    std::cout << "variadic\n";
}

int main() {
    // A< int > x;
    // A< double, void > b;
    int a;
    _S_create();
    _S_create(a);
    _S_create(1, 2.0);
    return 0;
}
