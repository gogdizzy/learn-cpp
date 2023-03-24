#include <stdio.h>
#include <stdlib.h>

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
	printf( "How large is space(kiB)?\n" );
	int space;
	scanf( "%d%*c", &space );

	size_t allocMem = getAvailMem() - space * 1024;
	printf( "I want alloc %zu bytes\n", allocMem );
	char* p = (char*)malloc( allocMem );
	if( p == NULL ) {
		printf( "Fail malloc %zu bytes\n", allocMem );
		return -1;
	}
	for( char *q = p; q < p + allocMem; q += PAGE_SIZE ) q[0] = 0;
	printf( "press any key to quit!\n" );
	getchar();

	return 0;
}
