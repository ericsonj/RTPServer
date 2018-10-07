/**
 * File:   main.c
 * Author: Ericson Joseph
 *
 * Created on August  2, 2018,  8:27 PM
 */

#include "netmanager.h"
#include "rtcp.h"
#include "sys_time.h"
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
    uint8_t buffer[BUFFER_SIZE];

    struct sockaddr_in servaddr, cliaddr;

    NE_Init();
    NE_initTableRoute();

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

    char cname[32];

    while (1) {

        socklen_t len, n;

        bzero(buffer, BUFFER_SIZE);
        n = recvfrom(sockfd, (uint8_t *)buffer, BUFFER_SIZE, MSG_WAITALL,
                     (struct sockaddr *)&cliaddr, &len);

        if (RTCP_isSDES(buffer, n)) {
            RTCP_readCNAME(cname, buffer);
            NE_removeOlds();
            char *ip       = inet_ntoa(cliaddr.sin_addr);
            char *dateTime = getFormatTime();
            if (!NE_exist(cname)) {
                printf("%s\t%s\t%s:%d\tNEW.\n", dateTime, cname, ip,
                       cliaddr.sin_port);
                fflush(stdout);
                NE_put(cname, cliaddr);
            } else {
                printf("%s\t%s\t%s:%d\tKEEP-ALIVE.\n", dateTime, cname, ip,
                       cliaddr.sin_port);
                fflush(stdout);
                NE_touch(cname, cliaddr);
            }
        } else {
            if (buffer[0] != 0) {
                device_t *dscNE = NE_getDscNe(&cliaddr);
                if (dscNE != NULL) {
                    n = sendto(sockfd, (const char *)buffer, n, 0,
                               (struct sockaddr *)&dscNE->addr,
                               sizeof(struct sockaddr_in));
                }
            }
        }
    }

    return 0;
}
