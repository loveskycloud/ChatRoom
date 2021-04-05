#pragma once

#define MAX_CLIENT 50

struct Msg {
    char from[20];
    int flag;
    char message[512];
};

struct RecvMsg {
    struct Msg msg;
    int retval;
};


struct RecvMsg chat_recv(int *fd);
