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
        char buf[32] = {0};
        char kbuf[32] = "hello world";
        fd=open(argv[1],O_RDWR);
        if(fd < 0) {
                printf("open is error\n");
                return -1;
        } else {
                printf("open is ok\n");
        }
        if(read(fd,buf,sizeof(buf))) {
                printf("read is error\n");
                return -1;
        } else {
                printf("read from device is %s\n",buf);
        }
        if(write(fd,kbuf,sizeof(kbuf))) {
                printf("write is error\n");
                return -1;
        } else {
                printf("write is ok\n");
        }
        close(fd);
        return 0;
}