#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>


#define PORT (12345)


int main(int argc, char **argv)
{
    printf("This is a UDP client\n");

    int sock;
    if ( (sock=socket(AF_INET, SOCK_DGRAM, 0)) <0)
    {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_in addr; // IP address
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.100");
    addr.sin_port = htons(PORT);
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        exit(1);
    }


    char buff[512];
    int len = sizeof(addr);

    while (1)
    {
        gets(buff);

        int n;
        n = sendto(sock, buff, strlen(buff), 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 0)
        {
            perror("sendto error");
            close(sock);
            break;
        }

        n = recvfrom(sock, buff, 512, 0, (struct sockaddr *)&addr, (socklen_t*)&len);
        if (n>0)
        {
            buff[n] = 0;
            printf("received:");
            puts(buff);
        }
        else if (n==0)
        {
            printf("server closed\n");
            close(sock);
            break;
        }
        else if (n == -1)
        {
            perror("recvfrom error");
            close(sock);
            break;
        }
    }
    
    return 0;
}

