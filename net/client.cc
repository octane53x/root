// CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "3490"
#define MAXDATASIZE 100
const char* HOSTNAME = "rcbell";

void *get_in_addr(struct sockaddr *sa){
  if(sa->sa_family == AF_INET)
    return &(((struct sockaddr_in*)sa)->sin_addr);
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(){
  struct addrinfo hints, *servinfo, *p;
  int sockfd, nbytes;
  char s[INET6_ADDRSTRLEN], buf[MAXDATASIZE];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(HOSTNAME, PORT, &hints, &servinfo);

  for(p = servinfo; p != NULL; p = p->ai_next){
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if(sockfd == -1) continue;
    inet_ntop(p->ai_family,
        get_in_addr((struct sockaddr*)p->ai_addr), s, sizeof(s));
    int c = connect(sockfd, p->ai_addr, p->ai_addrlen);
    if(c == -1){
      close(sockfd);
      continue;
    }
    break;
  }
  inet_ntop(p->ai_family,
      get_in_addr((struct sockaddr*)p->ai_addr), s, sizeof(s));
  freeaddrinfo(servinfo);

  nbytes = recv(sockfd, buf, MAXDATASIZE-1, 0);
  buf[nbytes] = '\0';
  printf("Client: Received %s\n", buf);
  close(sockfd);
  return 0;
}
