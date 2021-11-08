#include "Tools.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 10000
#define LISTENQ 16
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

    printf("server is running...\n");
    return 0;
}

void ConnectClient(int listenfd, int *maxfd, int *client, int *nclient, fd_set *allset, sockaddr_in *cliaddr, int *nready) {
    int i;
    int connfd;
    socklen_t clilen;

    clilen = sizeof(cliaddr);
    connfd = accept(listenfd, (sockaddr *) cliaddr, &clilen);

    for (i = 0; i < FD_SETSIZE; i++) {
        if (client[i] == -1) {
            client[i] = connfd;
            break;
        }
    }
    if (i == FD_SETSIZE) {
        printf("Too many clients\n");
        exit(-1);
    }

    FD_SET(connfd, allset);
    if (connfd > *maxfd)
        *maxfd = connfd;
    if (i + 1 > *nclient)
        *nclient = i + 1;

    *nready--;
}

int main(int argn, char **argv) {
    int i,j;
    int listenfd, maxfd, sockfd;
    int nready, nclient, client[FD_SETSIZE];
    ssize_t n;
    fd_set ready, allset;
    sockaddr_in srvaddr, cliaddr;
    char ibuff[MAXLINE], obuff[MAXLINE], buff[MAXLINE];
    int port;

    sscanf(argv[1], "%d", &port);

    if (Init(&listenfd, &srvaddr, port) != 0) {
        perror("Server terminated.\n");
        return 0;
    }
    maxfd = listenfd;
    nclient = 0;
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for (;;) {
        ready = allset;
        nready = select(maxfd + 1, &ready, NULL, NULL, NULL); // select ready fd from all fd

        if (FD_ISSET(listenfd, &ready)) {
            ConnectClient(listenfd, &maxfd, client, &nclient, &allset, &cliaddr, &nready);
            if (nready <= 0)
                continue;
        }
        for (i = 0; i < nclient; i++) {
            if ( (sockfd = client[i]) == -1)
                continue;
            if (FD_ISSET(sockfd, &ready)) {
                n = read(sockfd, ibuff, MAXLINE);
                if (n == 0) {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    sprintf(obuff, "%s", obuff);
                    printf("received message: %s\n", obuff);
                    write(sockfd, obuff, strlen(obuff));
                }
            }
        }
    }

    return 0;
}
