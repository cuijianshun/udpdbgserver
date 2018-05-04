#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "log.h"

extern void writelog(const char *fileName, const char *log);

int main(int argc,char *argv[])
{
    int socket_fd, file_fd;
    struct sockaddr_in netaddr;
    struct sockaddr_in fromaddr;
    socklen_t len = sizeof(struct sockaddr);
    char *path = "/home/cuijianshun/udpdbgserver/log/log.txt";

    memset(&netaddr, 0, len);
    netaddr.sin_family = AF_INET;
    netaddr.sin_port = htons(5000);
    netaddr.sin_addr.s_addr = INADDR_ANY;

    if((socket_fd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("Fail to create socket");
        return -1;
    }
    if (bind(socket_fd, (struct sockaddr*)&netaddr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }


    while (1) {
        char buf[1024] = {};
        int ret = recvfrom(socket_fd, buf, 1024, 0, (struct sockaddr *)&fromaddr, &len);
        if (ret == -1) {
            perror("recvfrom");
            exit(1);
        }
        printf("buf=%s\n", buf);
        writelog(path, buf);
        if (fork() == 0)

            break;
        printf("next==============\n");
    }
    int sock2;
    if((sock2 = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("child Fail to create socket");
        return -1;
    }
    if (sendto(sock2, "OK", 5, 0, (struct sockaddr*)&fromaddr, len) == -1) {
        perror("child sendto");
        exit(1);
    }
    printf("send OK\n");

    while (1) {
        char buf[1024] = {};
        int ret = recvfrom(sock2, buf, 1024, 0, (struct sockaddr *)&fromaddr, &len);
        if (ret == -1) {
            perror("child recvfrom");
            exit(1);
        }
        printf("child buf=%s\n", buf);
        writelog(path, buf);
    }

    close(socket_fd);

    return 0;
}
