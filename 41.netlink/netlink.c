#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>

int main(int argc, char *argv[]) {
        int ret;
        struct sockaddr_nl nl; 
        int len = 0;
        char buf[4096] = {0}; 
        bzero(&nl, sizeof(struct sockaddr_nl)); 
        nl.nl_family = AF_NETLINK; 
        nl.nl_pid = 0; 
        nl.nl_groups = 1; 
        int socket_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT); // 创建一个 Netlink套接字
        if (socket_fd < 0) {
                printf("socket error\n");
                return -1;
        }
        ret = bind(socket_fd, (struct sockaddr *)&nl, sizeof(struct sockaddr_nl)); // 使用 bind 函数将socket_fd 套接字与 nl 地址结构体绑定在一起
        if (ret < 0) {
                printf("bind error\n");
                return -1;
        }

        while (1) {
                bzero(buf, 4096); // 将缓冲区 buf 清零，确保数据接收前的初始化
                len = recv(socket_fd, &buf, 4096, 0); 
                for (i = 0; i < len; i++) {        // 将接收到的数据中的 '\0' 字符替换为 '\n' 字符，以便于输出,不然输出的信息不完整
                        if (*(buf + i) == '\0') {
                        buf[i] = '\n';
                }
                }
                printf("%s\n", buf); // 打印接收到的数据
        }
        return 0;
}