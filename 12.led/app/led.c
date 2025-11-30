#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//./a.out /dev/device_number 1 这样使用
int main(int argc,char *argv[])
{
        int fd;
        char buf[32] = {0};
        fd = open(argv[1], O_RDWR);
        if (fd < 0) {
                printf("open is error\n");
                return -1;
        }
        printf("open is ok\n");
        buf[0] =atoi(argv[2]);
        printf("buf[0] = %d\n",buf[0]);
        write(fd, buf, sizeof(buf));
        close(fd);
        return 0;
}