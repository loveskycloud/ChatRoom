#include "head.h"

struct User{
    char name[20];
    int online;
    pthread_t tid;
    int fd;
};

struct User *client;

void send_all(struct RecvMsg msg) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        /*
        if (client[i].online && strcmp(msg.msg.from, client[i].name)) {
            chat_send(msg, &client[i].fd);
        }
        */
        if (client[i].online) {
            chat_send(msg, &client[i].fd);
        }
    }
    return ;
}

int send_to(struct RecvMsg msg) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (client[i].online && !strcmp(client[i].name, msg.msg.to)) {
            chat_send(msg, &client[i].fd);
            return 1;
        }
    }
    return -1;
}

bool check_online(char *name) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (client[i].online && !strcmp(client[i].name, name)) {

            printf("D: %s is online\n", client[i].name);

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



void *work(void *arg) {
    struct User *client = (struct User *)arg;  
    struct RecvMsg msg;
    printf("%s login\n", client->name);
    while (1) {
        msg = chat_recv(&client->fd);
            if (msg.retval < 0) {
                printf("%s logout!\n", client->name);
                close(client->fd);
                client->online = 0;
                return NULL;
            }
            printf("%s : %s\n", msg.msg.from, msg.msg.message);

            if (msg.msg.flag == 0) {
                send_all(msg);
            } else if (msg.msg.flag == 1) {
                if(send_to(msg) < 0) {
                    msg.msg.flag = 2;
                    strcpy(msg.msg.from, "system");
                    strcpy(msg.msg.message, "The friend not online");
                    chat_send(msg, &client->fd);
                }
            } else {

            }
    }
    return (void *)NULL;
}

char *conf = (char *)"./conf/server.conf";


int main() {
    int port, server_listen, fd;
    port = atoi(get_value((char *)conf, (char *)"Server_Port"));
    client = (struct User *)calloc(MAX_CLIENT, sizeof(struct User));
    printf("port = %d, \n", port);
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
            printf("faild\n");
            close(fd);
            continue;
        }

        if (check_online(recvmsg.msg.from)) {
            //the user already online
            //FIXME
            close(fd);
            continue;
        } else {
            int sub;
            sub = find_sub();
            client[sub].online = 1;
            client[sub].fd = fd;
            strcpy(client[sub].name, recvmsg.msg.from);
            pthread_create(&client[sub].tid, NULL, work, (void *)&client[sub]);

        }
    }

    close(server_listen);
    return 0;
}

