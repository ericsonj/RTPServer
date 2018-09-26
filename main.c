/**
 * File:   main.c
 * Author: Ericson Joseph
 *
 * Created on August  2, 2018,  8:27 PM
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 19000
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {

    int sockfd;
    char buffer[BUFFER_SIZE];

    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket init error");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port        = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) <
        0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Send to ARM device
    struct sockaddr_in armaddr;
    memset(&armaddr, 0, sizeof(armaddr));
    armaddr.sin_family = AF_INET;
    armaddr.sin_port   = htons(19000);
    inet_aton("192.168.88.70", &armaddr.sin_addr);

    // Send to ANDROID device
    struct sockaddr_in androidaddr;
    memset(&androidaddr, 0, sizeof(androidaddr));
    androidaddr.sin_family = AF_INET;
    androidaddr.sin_port   = htons(19000);
    inet_aton("192.168.88.61", &androidaddr.sin_addr);


    while (1) {

        int len, n;

        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                     (struct sockaddr *)&cliaddr, &len);

        if (cliaddr.sin_addr.s_addr != armaddr.sin_addr.s_addr) {

            n = sendto(sockfd, (const char *)buffer, n, 0,
                       (struct sockaddr *)&armaddr, sizeof(armaddr));

        }else if(cliaddr.sin_addr.s_addr != androidaddr.sin_addr.s_addr){

            n = sendto(sockfd, (const char *)buffer, n, 0,
                       (struct sockaddr *)&androidaddr, sizeof(androidaddr));

        }

    }

    return 0;
}
