#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1000
#define LISTENQ 16

int main(int argn, char **argv) {
    sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char buff[MAXLINE];

    int port = atoi(argv[1]);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    printf("Port: %d\n", port);

    bind(listenfd, (sockaddr *) &servaddr, sizeof(servaddr));
    printf("[+]Bind\n");
    listen(listenfd, LISTENQ);
    printf("[+]Listening for the client\n");

    while (1) {
        int connfd = accept(listenfd, (sockaddr *) &cliaddr, &len);
        printf("connection from %s, port %d\n",
                inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
                ntohs(cliaddr.sin_port));
        time_t ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));
        close(connfd);
    }

    return 0;
}