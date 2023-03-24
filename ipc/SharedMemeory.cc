//
// Created by 关鑫 on 2021/2/7.
//

#include <stdio.h>
#include <sys/shm.h>

int main() {
    int shmid = shmget( 100, 4, IPC_CREAT );
    if( shmid < 0 ) {
        perror( "shmget" );
        return -1;
    }
    printf( "shmid = %d\n", shmid );


    int* p = (int*)shmat( shmid, NULL, 0 );
    if( (long)p < 0 ) {
        perror( "shmat" );
    }
    printf( "addr: %p\n", p );

    p = (int*)shmat( shmid, p, SHM_REMAP );
    if( (long)p < 0 ) {
        perror( "shmat" );
    }
    printf( "shmat( shmid, oldAddr, SHM_REMAP ): %p\n", p );

    p = (int*)shmat( shmid, NULL, 0 );
    if( (long)p < 0 ) {
        perror( "shmat" );
    }
    printf( "shmat( shmid, NULL, 0 ): %p\n", p );

    return 0;
}
