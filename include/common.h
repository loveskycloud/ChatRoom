#pragma once

#define MAX_CLIENT 50

struct Msg {
    char from[20];
    char to[20];
    int flag; // 0 1 2 
    char message[512];
};

struct RecvMsg {
    struct Msg msg;
    int retval;
};


struct RecvMsg chat_recv(int *fd);
void chat_send(struct RecvMsg msg, int *fd);
