//
// Created by 关鑫 on 2020/9/8.
//

#include <cstdint>
#include <cstdio>

int main() {
    {
        int a;
        scanf( "%*d%d", &a ); // discard first int, assign second int to a
    }
    {
        int8_t a;
        int16_t b;
        int64_t c;
        long d;
        scanf( "%hhd", &a );
        scanf( "%hd", &b );
        scanf( "%lld", &c ); // qd before
        scanf( "%ld", &d );
    }
    {
        intmax_t a;
        std::ptrdiff_t b;
        size_t c;
        scanf( "%jd", &a );
        scanf( "%td", &a );
        scanf( "%zd", &a );
    }
    {
        float a;
        double b;
        long double c;
        scanf( "%f", &a );
        scanf( "%lf", &b );
        scanf( "%Lf", &c );
    }
    {
        char a;
        char b[10];
        wchar_t c;
        wchar_t d[10];
        scanf( "%c", &a );
        scanf( "%s", &b );
        scanf( "%lc", &c );
        scanf( "%ls", &d );
    }
    {
        int totalCharCount;
        int a, b;
        scanf( "%d%d%n", &a, &b, &totalCharCount ); // 输入"123 4567", totalCharCount会是8，因为前面读入了8个字符，这不是一个conversion，不会计入函数返回值
    }
    {
        char a;
        scanf( "%*[ \n]%c", &a ); // 过滤前面的空格和回车
    }

    return 0;
}
