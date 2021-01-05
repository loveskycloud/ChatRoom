#include "head.h"

int sockfd;

void logout(int signalnum) {
    
    printf("recv a signal\n");
    close(sockfd);
    exit(0);
    return ;
}



int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage %s ip port\n", argv[0]);
        exit(1);
    }

    int port;
    port = atoi(argv[2]);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(argv[1]);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    printf("Socket Create Success!\n");

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("Connect Success!\n");

    
    signal(SIGINT, logout);

    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    char msg[512] = {0};
      if (pid == 0) {
        while (1) {
            printf("INPUT MESSAGES:\n");
            scanf("%s", msg);
            if (send(sockfd, msg, strlen(msg), 0) <= 0) {
                perror("send");
                continue;
            }

            if (recv(sockfd, msg, 1024, 0) <= 0) {
                perror("recv");
                continue;
            }
            printf("strlen : %d\n", strlen(msg));
            printf("Recv Enter: %s\n", msg);
            fflush(stdin);
            fflush(stdout);
        }
    } else {
        wait(NULL);
    } 
    close(sockfd);
    return 0;
}

