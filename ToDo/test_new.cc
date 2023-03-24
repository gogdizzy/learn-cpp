#include <cstdio>

int main() {
	int *p = new int[100];
	for( int i = 0; i < 100; ++i ) printf( "%d ", p[i] );
	delete[] p;
	return 0;
}
