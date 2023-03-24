#include <stdio.h>
#include <malloc/malloc.h>

#include <chrono>
#include <functional>

class benchmark {
    using fun = std::function< void() >;
public:
    benchmark( int catalog_width = 16, int item_width = 16 ) : catalog_width( catalog_width ), item_width( item_width ) {}

    void head( const char *catalog, std::initializer_list< const char * > heads ) {
        printf( "%*s", catalog_width, catalog );
        for( auto h : heads ) {
            printf( "%*s", item_width, h );
        }
        printf( "\n" );
    }

    void content( const char *catalog, std::initializer_list< const fun > funs ) {
        printf( "%*s", catalog_width, catalog );
        for( auto &f : funs ) {
            auto beg = std::chrono::high_resolution_clock::now();
            f();
            auto end = std::chrono::high_resolution_clock::now();
            printf( "%*lld", item_width, std::chrono::duration_cast< std::chrono::microseconds >(end - beg).count() );
        }
        printf( "\n" );
    }

private:
    int catalog_width;
    int item_width;
};

void testMmap( int N ) {
	mallopt( M_MMAP_THRESHOLD, 64 * 1024 );
	for( int i = 0; i < N; ++i ) {
		char *p = (char*)malloc( 128 * 1024 );
		free( p );
	}
}

void testSbrk( int N ) {
	mallopt( M_MMAP_THRESHOLD, 512 * 1024 );
	for( int i = 0; i < N; ++i ) {
		char *p = (char*)malloc( 128 * 1024 );
		free( p );
	}
}
int main() {
	int N = 10000;
	benchmark bm( 20, 20 );
    bm.head( "", { "sbrk", "mmap" } );
    bm.content( "time", { [=]() { testSbrk( N ); }, [=]() { testMmap( N ); } } );
	return 0;
}
