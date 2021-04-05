#include "head.h"

struct RecvMsg chat_recv(int *fd) {
    struct RecvMsg tmp;
    memset(&tmp, -1, sizeof(tmp));
    if (recv(*fd, &tmp.msg, sizeof(struct Msg), -1) <= 0) {
        tmp.retval = -2;
    } 
    return tmp;
}
