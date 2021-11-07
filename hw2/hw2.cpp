#include "Tools.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 10000
#define LISTENQ 16
#define FD_SETSIZE 16
// #define PORT 8888

using namespace std;

int Init(int *listenfd, sockaddr_in *srvaddr, int port) {
    *listenfd = socket(AF_INET, SOCK_STREAM, 0);
    srvaddr->sin_family = AF_INET;
    srvaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr->sin_port = htons(port);

    if (bind(*listenfd, (sockaddr *) srvaddr, sizeof(*srvaddr)) == 0)
        printf("[+]bind\n");
    else
        return -1;
    if (listen(*listenfd, LISTENQ) == 0)
        printf("[+]listen\n");
    else
        return -1;

    printf("server is running\n");
    return 0;
}

int main(int argn, char **argv) {
    int i,j;
    int listenfd, maxi, maxfd, connfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    sockaddr_in srvaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char ibuff[MAXLINE], obuff[MAXLINE], buff[MAXLINE];
    int port;

    sscanf(argv[1], "%d", &port);

    if (Init(&listenfd, &srvaddr, port) != 0) {
        perror("Server terminated.\n");
        return 0;
    }
    maxfd = listenfd;
    maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }
    fd_zero(&allset);
    fd_set(listenfd, &allset);

    for (;;) {
        rset  = allset;
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        if (fd_isset(listenfd, &rset)) {
            len = sizeof(cliaddr);
            connfd = accept(listenfd, (sockaddr *) &cliaddr, &clilen);

            for (i = 0; i < fd_setsize; i++) {

            }
        }
    }

    return 0;
}
