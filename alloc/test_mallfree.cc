#include <malloc.h>
#include <stdio.h>

size_t getMallFree()
{
#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    struct mallinfo info = mallinfo();
    return info.fordblks;
#else
    return 0;
#endif
}

int main() {
	long long s = 0;
	int n;
	scanf( "%d", &n );
	while( n-- ) s += getMallFree();
	printf( "%lld\n", s );
	return 0;
}
