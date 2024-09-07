#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
int sockfd1, sockfd2;
struct sockaddr_in addr1, addr2;
fd_set readfds;
int maxfd;
char buffer[1024];

sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
memset(&addr1, 0, sizeof(addr1));
memset(&addr2, 0, sizeof(addr2));

addr1.sin_family = AF_INET;
addr1.sin_port = htons(8080);
addr1.sin_addr.s_addr = htonl(INADDR_ANY);

addr2.sin_family = AF_INET;
addr2.sin_port = htons(8081);
addr2.sin_addr.s_addr = htonl(INADDR_ANY);

bind(sockfd1, (struct sockaddr*)&addr1, sizeof(addr1));
bind(sockfd2, (struct sockaddr*)&addr2, sizeof(addr2));

listen(sockfd1, 5);
listen(sockfd2, 5);

FD_ZERO(&readfds);
FD_SET(sockfd1, &readfds);
FD_SET(sockfd2, &readfds);

maxfd = sockfd1 > sockfd2 ? sockfd1 : sockfd2;

while (1) {
 select(maxfd + 1, &readfds, NULL, NULL, NULL);

 if (FD_ISSET(sockfd1, &readfds)) {
 int connfd = accept(sockfd1, NULL, NULL);
 read(connfd , buffer , 1024);
 printf("Received from socket 1: %s\n", buffer);
 close(connfd);
}

 if (FD_ISSET(sockfd2, &readfds)) {
  int connfd = accept(sockfd2 , NULL , NULL);
  read(connfd , buffer , 1024);
  printf("Received from socket 2: %s\n", buffer);
  close(connfd);
 }
}

return 0;
}