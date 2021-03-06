#include "Handle.hpp"
#include "Protocal.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 10000
#define LISTENQ 16
#define TEST false
// #define PORT 8888

using namespace std;

int Init(int *listenfd, int *udpfd, sockaddr_in *srvaddr, int port) {
    *listenfd = socket(AF_INET, SOCK_STREAM, 0);
    *udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    srvaddr->sin_family = AF_INET;
    srvaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr->sin_port = htons(port);

    int flag = 1;
    if (setsockopt(*listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) != -1)
        printf("[+] SO_REUSEADDR\n");
    else
        printf("ERR: SO_REUSEADDR fail\n");

    if (bind(*listenfd, (sockaddr *) srvaddr, sizeof(*srvaddr)) == 0)
        printf("[+]bind TCP listen fd = %d\n", *listenfd);
    else
        return -1;

    if (bind(*udpfd, (sockaddr *) srvaddr, sizeof(*srvaddr)) == 0)
        printf("[+]bind UDP fd = %d\n", *udpfd);
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
    int i , code;
    int listenfd, udpfd, maxfd, sockfd;
    int nready, nclient, client[FD_SETSIZE];
    ssize_t n;
    fd_set ready, allset;
    sockaddr_in srvaddr, cliaddr;
    socklen_t clilen;
    message_pk mesgpk;
    message_t mesg;
    char ibuff[MAXLINE], obuff[MAXLINE], buff[MAXLINE];
    int port;

    if (argn > 3) {
        printf("usage: ./hw3 [port number]\n");
        return 0;
    }

    if (argn == 2) {
        sscanf(argv[1], "%d", &port);
    } else {
        port = 7890;
    }

    if (Init(&listenfd, &udpfd, &srvaddr, port) != 0) {
        perror("Server terminated.\n");
        return 0;
    }
    maxfd = max(listenfd, udpfd);
    nclient = 0;
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    FD_SET(udpfd, &allset);

    for (;;) {
        ready = allset;
        nready = select(maxfd + 1, &ready, NULL, NULL, NULL); // select ready fd from all fd

        if (FD_ISSET(listenfd, &ready)) {
            sockfd = ConnectClient(listenfd, &maxfd, client, &nclient, &allset, &cliaddr);
            printf("Connect with a client through socket (fd = %d)\n", sockfd);
            strcpy(obuff, "********************************\n");
            strcat(obuff, "** Welcome to the BBS server. **\n");
            strcat(obuff, "********************************\n");
            strcat(obuff, "% ");
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
                    if (TEST)
                        write(sockfd, ibuff, strlen(ibuff));

                    code = Handle(sockfd, TCP, ibuff, NULL, obuff, sizeof(obuff), cliaddr);
                    write(sockfd, obuff, strlen(obuff));
                    if (code == 0) {
                        snprintf(obuff, sizeof(obuff), "%% ");
                        write(sockfd, obuff, strlen(obuff));
                    } else {
                        close(sockfd);
                        FD_CLR(sockfd, &allset);
                        printf("Closed socket (fd = %d)\n", sockfd);
                        client[i] = -1;
                    }
                }

                if (--nready <= 0)
                    break;
            }
        }

        if (FD_ISSET(udpfd, &ready)) {
            clilen = sizeof(cliaddr);
            n = recvfrom(udpfd, ibuff, MAXLINE, 0, (sockaddr *) &cliaddr, &clilen);
            ibuff[n] = '\0';
            printf("UDP packet received. From %s:%d. Length = %d\n",
                inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
                ntohs(cliaddr.sin_port),
                (int) n);
            genpack(ibuff, n, &mesgpk);
            unpack_message(&mesg, &mesgpk);
            snprintf(buff, MAXLINE, "chat\n");
            printf("Call [%s] to event handler\n", buff);
            code = Handle(udpfd, UDP, buff, &mesg, obuff, sizeof(obuff), cliaddr);
        }
    }

    return 0;
}
