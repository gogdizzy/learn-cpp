//
// Created by 关鑫 on 2019/10/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char **argv) {

    int sig = 0;

    if( argc > 1 ) sig = atoi( argv[1] );

    void* dh = nullptr;
#if defined(__APPLE__)
    dh = dlopen( "libmulti-thread.dylib", RTLD_GLOBAL | RTLD_NOW );
#else
    dh = dlopen( "./libmulti-thread.so", RTLD_GLOBAL | RTLD_NOW );
#endif
    if( !dh ) {
        printf( "open fail: %s\n", dlerror() );
        return -1;
    }

    auto func = (void (*)(int))dlsym( dh, "test" );
    if( !func ) {
        printf( "find func fail\n" );
        return -2;
    }

    func( sig );

    dlclose( dh );
    return 0;
}