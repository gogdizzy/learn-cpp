//
// Created by 关鑫 on 2021/9/3.
//

#include <stdio.h>
#include <functional>

typedef int (*BaseFuncType)(const int);
// typedef BaseFuncType (*FuncType)(FuncType &&);

int haha(const int) {
    return 10;
}

//int badSyntax(int n) {
//    auto sum = [](const int n) {
//        return n == 1 ? 1 : n + sum(n - 1);
//    };
//    return sum(n);
//}

int useStdFunction(int n) {
    std::function<int(const int)> sum;
    sum = [&](const int n) {
        return n == 1 ? 1 : n + sum(n - 1);
    };
    return sum(n);
}

int selfAsParameter(int n) {
    auto sum = [](auto&& self, const int n) -> int {
        return n == 1 ? 1 : n + self(self, n - 1);
    };
    return sum(sum, n);
}

int yCombinator(int n) {
    const auto& y = [](const auto& fn) {
        return [&](const auto& f) {
            return f(f);
        }([&](const auto& f) -> std::function<int(const int)> {
            return fn([&](const int n) {
                return f(f)(n);
            });
        });
    };

    const auto& sum = [](const auto& f) {
        return [&](const int n) {
            return n == 1 ? 1 : n + f(n - 1);
        };
    };
    return y(sum)(n);
}

int yCombine(int n) {
    auto sum = [&](const auto& f) { return f(f); }([&](const auto& f) -> std::function<int(const int)> {
        return [&](const int n) { return n == 1 ? 1 : n + f(f)(n - 1); };
    });
    return sum(n);
}


int main() {

    FuncType xx(haha);

    printf("%d\n", xx(1));

    printf("%d\n", useStdFunction(10));
    printf("%d\n", selfAsParameter(10));
    printf("%d\n", yCombinator(10));
    printf("%d\n", yCombine(10));

    return 0;
}
