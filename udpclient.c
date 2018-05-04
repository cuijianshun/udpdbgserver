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

int main(int argc,char *argv[])
{
    int socket_fd;

    struct sockaddr_in fromaddr;
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(struct sockaddr);

    char *path = "/home/cuijianshun/udpdbgserver/errtext/errtext.txt";
    FILE *fp;
    fp = fopen(path, "r");
    //char errbuf[1024] = {};
    //fgets(errbuf, 1024, fp);

    if((socket_fd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("Fail to create socket");
        return -1;
    }


    memset(&servaddr, 0, len);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5000);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    char errbuf[100] = {};
    while(fgets(errbuf, 100, fp) != NULL){
        if (sendto(socket_fd, errbuf, 100, 0, (struct sockaddr*)&servaddr, len) == -1) {
            perror("sendto");
            exit(1);
        }
       printf("errbuf : %s\n", errbuf);
       memset(errbuf, 0, sizeof(errbuf));

    char buf[100] = {};
    if (recvfrom(socket_fd, buf, 100, 0, (struct sockaddr*)&fromaddr, &len) == -1) {
        perror("recvfrom");
        exit(1);
    }
    printf("recvfrom ip %s : %d\n", inet_ntoa(fromaddr.sin_addr), ntohs(fromaddr.sin_port));
    printf("buf=%s\n", buf);

    }
/*
    int i;
    for (i = 0; i < 10; i++) {
        char buferr[100] = {};
        fgets(buferr, 1024, fp);

        if (sendto(socket_fd, buferr, 100, 0, (struct sockaddr*)&fromaddr, len) == -1) {
            perror("sendto");
            exit(1);
        }
        printf("send ok\n");
        sleep(1);
    }
*/
    //sleep(5);
   close(socket_fd);
   return 0;
}
