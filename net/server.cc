// SERVER

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
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"
#define BACKLOG 10

void sig_handler(int s){
  int save = errno;
  while(waitpid(-1, NULL, WNOHANG) > 0);
  errno = save;
}

void *get_in_addr(struct sockaddr *sa){
  if(sa->sa_family == AF_INET)
    return &(((struct sockaddr_in*)sa)->sin_addr);
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(){
  struct addrinfo hints, *servinfo, *p;
  struct sigaction sa;
  struct sockaddr_storage their_addr;
  int sockfd, new_fd, yes=1;
  char s[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, PORT, &hints, &servinfo);

  for(p = servinfo; p != NULL; p = p->ai_next){
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if(sockfd == -1) continue;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    int b = bind(sockfd, p->ai_addr, p->ai_addrlen);
    if(b == -1){
      close(sockfd);
      continue;
    }
    break;
  }
  freeaddrinfo(servinfo);
  listen(sockfd, BACKLOG);
  printf("Starting server\n");

  sa.sa_handler = sig_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGCHLD, &sa, NULL);

  while(1){
    socklen_t sin = sizeof(their_addr);
    new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin);
    if(new_fd == -1) continue;
    inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr*)&their_addr), s, sizeof(s));
    printf("Server: Connection from %s\n", s);

    if(!fork()){
      close(sockfd);
      send(new_fd, "Hello client!", 13, 0);
      close(new_fd);
      exit(0);
    }
    close(new_fd);
  }
  return 0;
}
