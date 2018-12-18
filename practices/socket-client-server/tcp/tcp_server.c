#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#define SERVER_PORT 12345           // arbitrary but client & server must agree
#define BUF_SIZE    4096            // block transfer size
#define QUEUE_SIZE  10


int main(int argc, char** argv)
{
    int s, b, l, fd, sa, bytes;
    int on=1;
    char buf[BUF_SIZE];             // buffer for outgoing file
    struct sockaddr_in channel;     // holds IP address

    // Build address structure to bind to socket
    memset(&channel, 0, sizeof(channel));           // zero channel
    channel.sin_family = AF_INET;
    channel.sin_addr.s_addr = htonl(INADDR_ANY);    // convert between host byte and network byte
    channel.sin_port = htons(SERVER_PORT);

    // socket: Passive open - Wait for connection
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // create socket
    if(s < 0) perror("socket failed.");
    //
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));

    // // TODO 非阻塞设置
    // int flags = fcntl(s, F_GETFL, 0); //获取建立的socket的当前状态（非阻塞）
    // fcntl(s, F_SETFL, flags|O_NONBLOCK); //将当前sockfd设置为非阻塞

    // bind
    b = bind(s, (struct sockaddr *)&channel, sizeof(channel));
    if(b < 0) perror("bind failed.");

    l = listen(s, QUEUE_SIZE);                      // specify queue size
    if(l < 0) perror("listen failed.");

    // Wait for connection and process it
    while(1)
    {
        sa = accept(s, 0, 0);                       // block for connection request
        if(sa < 0) perror("accept failed.");

        read(sa, buf, BUF_SIZE);                    // read file name from socket

        // Get and return the file 
        fd = open(buf, O_RDONLY);
        if(fd < 0) perror("open failed.");

        while(1)
        {
            bytes = read(fd, buf, BUF_SIZE);
            if(bytes <= 0) break;                  // check for end of file

            write(sa, buf, bytes);                  // write bytes to socket
        }

        close(fd);
        close(sa);
    }

    return 0;
}
