#include "head.h"

struct User{
    char name[20];
    int online;
    pthread_t tid;
    int fd;
};

struct User *client;

bool check_online(char *name) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (client[i].online && !strcmp(client[i].name, name)) {
            return true;
        }
    }
    return false;
}

int find_sub() {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (!client[i].online) return i;
    }
    return -1;
}

void *work(void *) {
    printf("nothing\n");
    return (void *)NULL;
}

char *conf = (char *)"../conf/server.conf";


int main() {
    int port, server_listen, fd;
    port = htons(atoi(get_value((char *)conf, (char *)"Server_Ip")));
    client = (struct User *)malloc(sizeof(struct User) * MAX_CLIENT);
    memset(&client, 0, sizeof(client));

    if ((server_listen = create_tcp_socket(port)) < 0) {
        perror("create_tcp_socket");
        return 1;
    }

    while (1) {
        if ((fd = accept(server_listen, NULL, NULL)) < 0) {
            perror("accept");
            continue;
        }
        
        RecvMsg recvmsg = chat_recv(&fd);

        if (recvmsg.retval < 0) {
            close(fd);
            continue;
        }

        if (check_online(recvmsg.msg.from)) {
            //the user already online
            continue;
        } else {
            int sub;
            sub = find_sub();
            client[sub].online = 1;
            client[sub].fd = fd;
            strcpy(client[sub].name, recvmsg.msg.from);
            pthread_create(&client[sub].tid, NULL, work, NULL);
        }
    }

    close(server_listen);
    return 0;
}

