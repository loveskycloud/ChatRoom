#include "head.h"

char *conf = (char *)"./conf/client.conf";
char *logfile = (char *)"text";
struct Msg msg;

int sockfd;

void logout(int) {
    printf("You left the room\n");
    close(sockfd);
    exit(1);
    return ;
}

char *gettime() {
    time_t timep;
    time(&timep);
    return asctime(gmtime(&timep));
}

int main()
{

    int fd, port;
    //FILE *fp;
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
    
    signal(SIGINT, logout);

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        memset((char *)&msg, 0, sizeof(msg));
        strcpy(msg.from, get_value(conf, (char *)"Client_Name"));
        msg.flag = 2;
        fflush(stdin);
        char buff[512] = {0};
        while (1) {
            system("clear");
            memset(buff, 0, sizeof(buff));
            memset(msg.message, 0, sizeof(msg.message));
            printf("please input :\n");
            if (scanf("%[^\n]s", buff) == EOF) {
                exit(1);
            }
            getchar();

            strcpy(msg.message, buff);

            if (strlen(msg.message) <= 0) {
                continue;
            }

            if (buff[0] == '@') {
                //this is a private message
                //printf("private message\n");
                strncpy(msg.to, buff + 1, (int)(strstr(buff, " ") - buff - 1));
                msg.flag = 1;
            } else {
                msg.flag = 0;
            }

            if (send(sockfd, &msg, sizeof(msg), 0) <= 0) {
                perror("send");
                exit(1);
            }
        }
    } else {
        struct RecvMsg rmsg; 
        sleep(1);

        freopen(logfile, "a+", stdout);
        while (1) {
            memset(&rmsg, 0, sizeof(rmsg));
            rmsg = chat_recv(&sockfd);
            if (rmsg.retval < 0) {
                exit(0); 
            } 
            if (strlen(rmsg.msg.message) <= 0) {
                continue;
            }
            if (rmsg.msg.flag == 0) {
                //public message
                printf("%s : %s\n", rmsg.msg.from, rmsg.msg.message);
            } else if (rmsg.msg.flag == 1) {
                //private message
                printf("from %s private message : %s\n", rmsg.msg.from, rmsg.msg.message);
            } else if (rmsg.msg.flag == 2) {
                //system message
                printf("notice %s : %s\n", rmsg.msg.from, rmsg.msg.message);
            } else {
                //error
                printf("Error message\n");
            }

            fflush(stdout);
        }
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

