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

int ConnectClient(int listenfd, int *maxfd, int *client, int *nclient, fd_set *allset, sockaddr_in *cliaddr) {
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
    return connfd;
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
            sockfd = ConnectClient(listenfd, &maxfd, client, &nclient, &allset, &cliaddr);
            printf("Connect with a client through socket (fd = %d)\n", sockfd);
            strcpy(obuff, "********************************\n");
            strcat(obuff, "** Welcome to the BBS server. **\n");
            strcat(obuff, "********************************\n");
            write(sockfd, obuff, strlen(obuff));
            if (--nready <= 0)
                continue;
        }
        for (i = 0; i < nclient; i++) {
            if ((sockfd = client[i]) == -1)
                continue;
            if (FD_ISSET(sockfd, &ready)) {
                n = read(sockfd, ibuff, MAXLINE);
                if (n == 0) {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    printf("Closed socket (fd = %d)\n", sockfd);
                    client[i] = -1;
                } else {
                    ibuff[n] = '\0';
                    // snprintf(obuff, sizeof(obuff), "echo: %s", ibuff);
                    Handle(ibuff, obuff, sizeof(obuff));
                    // printf("received message: %s\n", ibuff);
                    // write(sockfd, obuff, strlen(obuff));
                }

                if (--nready <= 0)
                    break;
            }
        }
    }

    return 0;
}
