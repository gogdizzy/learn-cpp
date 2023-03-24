//
// Created by 关鑫 on 2021/4/14.
//
// 参考：https://blog.csdn.net/r1254/article/details/46914431

#include <cstdio>
#include <algorithm>

#define NUM_CNT 4
#define ANS 24

int gcd(int a, int b) {
    return a == 0 ? b : gcd(b % a, a);
}

struct Fraction {
    int numerator;
    int denominator;
    Fraction reduce() {
        int d = gcd(numerator, denominator);
        numerator /= d;
        denominator /= d;
        return *this;
    }
    Fraction operator+(const Fraction& other) const {
        return Fraction{this->numerator * other.denominator + this->denominator * other.numerator,
                        this->denominator * other.denominator}.reduce();
    }
    Fraction operator-(const Fraction& other) const {
        return Fraction{this->numerator * other.denominator - this->denominator * other.numerator,
                        this->denominator * other.denominator}.reduce();
    }
    Fraction operator*(const Fraction& other) const {
        return Fraction{this->numerator * other.numerator,
                        this->denominator * other.denominator}.reduce();
    }
    Fraction operator/(const Fraction& other) const {
        return Fraction{this->numerator * other.denominator,
                        this->denominator * other.numerator}.reduce();
    }
};

int num[NUM_CNT];
int ops[NUM_CNT - 1];
char op_name[] = {'+', '-', '*', '/'};

void print_ans(int pos) {
    if(pos < 0) {
        printf("%d", num[pos + 1]);
        return;
    }
    int op = ops[pos];
    putchar('(');
    if(op < 0) {
        printf("%d", num[pos + 1]);
        putchar(op_name[-op]);
        print_ans(pos - 1);
    }
    else {
        print_ans(pos - 1);
        putchar(op_name[op]);
        printf("%d", num[pos + 1]);
    }
    putchar(')');
}

void dfs(int curPos, Fraction result) {
    if(curPos >= NUM_CNT) {
        if(result.denominator * ANS == result.numerator) {
            print_ans(NUM_CNT - 2);
            putchar('\n');
        }
        return;
    }
    ops[curPos - 1] = 0;
    dfs(curPos + 1, result + Fraction{num[curPos], 1});
    ops[curPos - 1] = 1;
    dfs(curPos + 1, result - Fraction{num[curPos], 1});
    ops[curPos - 1] = -1;
    dfs(curPos + 1, Fraction{num[curPos], 1} - result);
    ops[curPos - 1] = 2;
    dfs(curPos + 1, result * Fraction{num[curPos], 1});
    ops[curPos - 1] = 3;
    dfs(curPos + 1, result / Fraction{num[curPos], 1});
    ops[curPos - 1] = -3;
    dfs(curPos + 1, Fraction{num[curPos], 1} / result);
}

void calc24() {
    std::sort(num, num + NUM_CNT);
    do {
        dfs(1, {num[0], 1});
    }
    while(std::next_permutation(num, num + NUM_CNT));
}

int main() {
    for(int i = 0; i < NUM_CNT; ++i) scanf("%d", &num[i]);
    calc24();
    return 0;
}