#include <algorithm>
#include <stdio.h>

#define M_SQRT2 1.41421356237309504880168872420969808

int main() {
	float a[1000];
        int b[1000];
	int N = 5;
	int tot = 0;
	for( int i = 0; i < 100; ++i ) a[i] = 1000000.f;
	for( int i = 0; i <= N; ++i ) for( int k = 0; k <= N; ++k ) { a[tot] = 1.0 * i + M_SQRT2 * k; b[tot] = 10 * i + 14 * k; ++tot; }

	std::sort( a, a + tot );
	std::sort( b, b + tot );

        printf( "float\t\tint\tfloat-trunc\n" );
	for( int i = 0; i < tot; ++i ) printf( "%f\t%d\t%d\n", a[i], b[i], (int)(a[i]*10) );

	return 0;
}
