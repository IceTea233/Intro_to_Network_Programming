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



int main(int argn, char **argv) {
    sockaddr_in srvaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char ibuff[MAXLINE], obuff[MAXLINE], buff[MAXLINE];
    int port;
    sscanf(argv[1], "%d", &port);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr *) &srvaddr, sizeof(srvaddr)) == 0)
        printf("[+]bind\n");
    if (listen(sockfd, LISTENQ) == 0)
        printf("[+]listen\n");
    printf("server is running\n");

    for (;;) {
        int connfd;
        if ((connfd = accept(sockfd, (sockaddr *) &cliaddr, &len)) != -1) {
            printf("Connect from %s:%d\n",
            inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
            ntohs(cliaddr.sin_port));

            snprintf(obuff, sizeof(obuff), "********************************\n** Welcome to the BBS server. **\n********************************\n");
            write(connfd, obuff, strlen(obuff));

            for (;;) {
                try {
                    snprintf(obuff, sizeof(obuff), "%% ");
                    write(connfd, obuff, strlen(obuff));
                    memset(ibuff, 0, sizeof(ibuff));
                    read(connfd, ibuff, sizeof(ibuff));
                    if (strlen(ibuff) == 0)
                        throw "Empty input.";

                    int code = Handle(ibuff, obuff, sizeof(obuff));
                    write(connfd, obuff, strlen(obuff));
                    if (code == 1)
                        break;
                } catch(...) {
                    printf("Unexpected event.\n");
                    break;
                }

            }
            close(connfd);
        }

        printf("Disonnect from %s:%d\n",
            inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
            ntohs(cliaddr.sin_port));
    }

    return 0;
}
