#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define MAXLINE 1000
#define LISTENQ 16
#define PORT 8888

using namespace std;

vector<string> getarg(char *input, int len) {
    stringstream ss;
    ss << input;
    string str;
    vector<string> ret;
    while (ss >> str) {
        ret.push_back(str);
    }
    return ret;
}

int handle(char *input, int len) {
    int ret = 0;
    vector<string> arg = getarg(input, len);
    cout << "size = " << arg.size() << "\n";
    for (auto it : arg) {
        cout << ">>> " << it << "\n";
    }
    return ret;
}

int main(int argn, char **argv) {
    sockaddr_in srvaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char ibuff[MAXLINE], obuff[MAXLINE], buff[MAXLINE];
    int port = PORT;

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
                snprintf(obuff, sizeof(obuff), "%% ");
                write(connfd, obuff, strlen(obuff));
                memset(ibuff, 0, sizeof(ibuff));
                read(connfd, ibuff, sizeof(ibuff));
                handle(ibuff, strlen(ibuff));
            }
        }
        close(connfd);
    }

    return 0;
}
