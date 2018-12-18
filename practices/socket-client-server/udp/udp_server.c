#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>


#define PORT (12345)


int main(int argc, char **argv)
{
    printf("Welcome! This is a UDP server, I can only received message from client and reply with same message\n");
    
    struct sockaddr_in addr; // holds UDP IP address
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    int sock;
    if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket error");
        exit(1);
    }
    // // Not block
    // int flags = fcntl(sock, F_GETFL, 0); // 获取当前状态
    // fcntl(sock, F_SETFL, flags|O_NONBLOCK); // 非阻塞设置
    // Bind
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind error");
        exit(1);
    }

    int n;
    char buff[512];
    struct sockaddr_in clientAddr; // client address
    int len = sizeof(clientAddr);

    while (1)
    {
        n = recvfrom(sock, buff, 511, 0, (struct sockaddr*)&clientAddr, (socklen_t*)&len);
        if (n>0)
        {
            buff[n] = 0;

            printf("%s %u says: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);

            n = sendto(sock, buff, n, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
            if (n < 0)
            {
                perror("send error");
                break;
            }

        }
        else
        {
            perror("recv");
            break;
        }
    }

    return 0;
}

