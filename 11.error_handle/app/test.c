#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


//./a.out /dev/device_number  这样使用
int main(int argc,char *argv[])
{
        int fd;
        char buf[32];
        fd=open(argv[1],O_RDWR,0666);
        if(fd < 0) {
                printf("open is error\n");
                return -1;
        }
        printf("open is ok\n");
        close(fd);
        return 0;
}