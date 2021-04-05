#include "head.h"

char *conf = (char *)"./conf/client.conf";

struct Msg msg;

int main()
{

    int fd, port;
    int sockfd;
    port = atoi(get_value(conf, (char *)"Client_Port"));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server;
    server.sin_port = htons(port);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr((char *)get_value(conf, (char  *)"Client_Ip"));
    printf("Port = %d, Ip = %s\n", port, get_value(conf, (char  *)"Client_Ip"));
    
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        exit(1);
    }
    printf("connect success\n");

    memset((char *)&msg, 0, sizeof(msg));
    strcpy(msg.from, get_value(conf, (char *)"Client_Name"));
    msg.flag = 2;

    send(sockfd, &msg, sizeof(msg), 0);

    pid_t pid;


    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        memset((char *)&msg, 0, sizeof(msg));
        strcpy(msg.from, get_value(conf, (char *)"Client_Name"));
        msg.flag = 2;
        fflush(stdin);
        while (1) {
            memset(msg.message, 0, sizeof(msg.message));
            printf("please input :");
            scanf("%[^\n]s", msg.message);
            getchar();
            if (strlen(msg.message) <= 0) {
                continue;
            }
            if (send(sockfd, &msg, sizeof(msg), 0) < 0) {
                perror("send");
                exit(1);
            }
        }
    } else {
        wait(NULL);
    }

/*
    while (true) {
        printf("please input : ");
        scanf("%[^\n]s", msg.message);
        getchar();
        if (send(sockfd, &msg, sizeof(msg), 0) < 0) {
            perror("send");
            exit(1);
        }
    }
*/
    close(sockfd);
    return 0;
}

