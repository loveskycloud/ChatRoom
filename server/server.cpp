#include "head.h"
#include "create_socket.h"

using namespace std;
struct node{
    int fd;
    pthread_t tid;
}node[1000];

void *do_pthread(void *arg) {
    
    int *fd = (int *)arg;
    
    char msg[512];

    while (1) {
        if (recv(*fd, msg, sizeof(msg), 0) <= 0) {
            printf("Client is Logout!\n");
            close(*fd);     
            return NULL;
        } 
        if (send(*fd, msg, sizeof(msg), 0) <= 0 ) {
        
            close(*fd); 
        }
        memset(msg, 0, sizeof(msg));
    }

    close(*fd);
    free(fd);
    return NULL;

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage %s port\n", argv[0]);
        exit(1);
    }     
    
    int sockfd, port;
    
    port = atoi(argv[1]);

    sockfd = create_socket(port);
    struct linger m_linger;

    if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &m_linger, (socklen_t)sizeof(m_linger)) < 0) {
        return -1;
    }

    int flag = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) < 0) {
        return -1;
    }
    for(int i = 0; i < 100; i++) {
        node[i].fd = -1;
    }

    //pthread_t tid;
    int t = 0;
    while (1) {
        int *fd = (int *)malloc(sizeof(int));
        if ((*fd = accept(sockfd, NULL, NULL)) < 0) {
            perror("accept");
            continue;
        }
        pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t));
        pthread_create(tid, NULL, do_pthread, (void *)fd); 
        printf("New client Login!\n");
        
    }
    return 0;
}

