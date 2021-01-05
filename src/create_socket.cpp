#include "create_socket.h"

int create_socket(int port) {
   int sockfd;
    
   struct sockaddr_in server;
   server.sin_family = AF_INET;
   server.sin_port = htons(port);
   server.sin_addr.s_addr = INADDR_ANY;

   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
       perror("socket");
       exit(1);
   }

   printf("Socket Create Success!\n");

   if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
       perror("bind");
       exit(1);
   }
   printf("Bind Success!\n");

   if (listen(sockfd, 25) < 0) {
       perror("listen");
       exit(1);
   }
   printf("Listen Success\n");

   return sockfd;
}
