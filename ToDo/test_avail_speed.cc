#include <stdio.h>
#include <stdlib.h>
#include <chrono>

size_t getAvailMem() {
        long free = 0L, buffer = 0L, cached = 0L;
	FILE* fp = NULL;
	if ( (fp = fopen( "/proc/meminfo", "r" )) == NULL )
		return (size_t)0L;		/* Can't open? */

	if ( fscanf( fp, "%*s%*s%*s" "%*s%ld%*s" "%*s%ld%*s" "%*s%ld%*s", &free, &buffer, &cached ) != 3 )
	{
		fclose( fp );
		return (size_t)0L;		/* Can't read? */
	}
	fclose( fp );
	return (size_t)(free + buffer + cached) * 1024L;
}

#define PAGE_SIZE (4*1024)

int main() {

	printf( "repeat times?\n" );
	int repeat;
	scanf( "%d%*c", &repeat );

auto beg = std::chrono::high_resolution_clock::now();	
	for( int i = 0; i < repeat; ++i ) size_t allocMem = getAvailMem();
auto end = std::chrono::high_resolution_clock::now();
        printf( "%lld", std::chrono::duration_cast< std::chrono::microseconds >(end - beg).count() );

	return 0;
}
