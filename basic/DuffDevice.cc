//
// Created by 关鑫 on 2020\9\12.
//

#include <stdint.h>
#include <stdio.h>
#include <memory.h>

#ifdef _MSC_VER
#  include <intrin.h>
#  pragma intrinsic __rdtsc
#  define unlikely(x) x
#else
#  define __rdtsc __builtin_ia32_rdtsc
#  define unlikely(x) __builtin_expect(!!(x), 0)
#endif

int A[1000000];
float B[1000000];
double C[1000000];

template <typename T>
void memsetSimple( T* begin, int count, const T& value )
{
    T* end = begin + count;
    while( unlikely(begin != end) ) *begin++ = value;
}

template <typename T>
void memsetPower( T* begin, int count, const T& value )
{
    int size = 1, mid = count / 2;
    *begin = value;
    while( size <= mid )
    {
        memcpy( begin + size, begin, sizeof(T) * size );
        size *= 2;
    }
    memcpy( begin + size, begin, sizeof(T) * (count - size) );
}

template <typename T>
void memsetDuff( T* begin, int count, const T& value )
{
    T* end = begin + count;
    switch( count & 7 )
    {
        case 0:while( unlikely(begin != end) ){
                    *begin++ = value;
        case 7:		*begin++ = value;
        case 6:		*begin++ = value;
        case 5:		*begin++ = value;
        case 4:		*begin++ = value;
        case 3:		*begin++ = value;
        case 2:		*begin++ = value;
        case 1:		*begin++ = value;}
    }
}

#define TEST( X, Y, Z ) \
t0 = __rdtsc();\
X( Y, sizes[i], Z );\
t1 = __rdtsc();\
printf("%-12lld\t", t1 - t0 )

int main()
{
    getchar();
    printf("DataTypeSize\tmemsetSimple\tmemsetPower\tmemsetDuff\n");
    int sizes[] = {100, 1000, 10000, 100000, 1000000};
    int i;
    int64_t t0, t1;

    for( i = 0; i < sizeof(sizes) / sizeof(int); i++ )
    {
        printf("int %8d\t", sizes[i] );
        TEST( memsetSimple, A, 1025 );
        TEST( memsetPower, A, 1025 );
        TEST( memsetDuff, A, 1025 );
        printf("\n");
    }

    for( i = 0; i < sizeof(sizes) / sizeof(int); i++ )
    {
        printf("flt %8d\t", sizes[i] );
        TEST( memsetSimple, B, 1025.f );
        TEST( memsetPower, B, 1025.f );
        TEST( memsetDuff, B, 1025.f );
        printf("\n");
    }

    for( i = 0; i < sizeof(sizes) / sizeof(int); i++ )
    {
        printf("dbl %8d\t", sizes[i] );
        TEST( memsetSimple, C, 1025. );
        TEST( memsetPower, C, 1025. );
        TEST( memsetDuff, C, 1025. );
        printf("\n");
    }

    return 0;
}
