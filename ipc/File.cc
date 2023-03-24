//
// Created by 关鑫 on 2021/11/12.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

    int fd = open(argv[1], O_RDONLY, 0);
    char c;

    if (fork() == 0) {
        read(fd, &c, 1);
        exit(0);
    }

    wait(NULL);

    read(fd, &c, 1);

    printf("c = %c\n", c);

    return 0;
}