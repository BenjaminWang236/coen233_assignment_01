/**
 * @file myserver.c
 * @author Benjamin Wang (bwang4@scu.edu, ID: 1179478)
 * @brief Client using customized protocol on top of UDP protocol for sending
 * information to the server. Server side implementation of UDP client-server.
 * @version 0.1
 * @date 2022-03-02
 *
 * @copyright Copyright (c) 2022
 * @source: https://www.linuxhowtos.org/C_C++/socket.htm
 *
 */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

/**
 * @brief Error function
 *
 * @param msg Error message
 */
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/**
 * @brief Main function
 *
 * @param argc number of arguments
 * @param argv arguments
 * @return int 0 if successful
 */
int main(int argc, char *argv[])
{
    int sock, length, n;
    socklen_t fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buf[1024];

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(0);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        error("Opening socket");
    length = sizeof(server);
    bzero(&server, length);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));
    if (bind(sock, (struct sockaddr *)&server, length) < 0)
        error("binding");
    fromlen = sizeof(struct sockaddr_in);
    while (1)
    {
        n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
        if (n < 0)
            error("recvfrom");
        write(1, "Received a datagram: ", 21);
        write(1, buf, n);
        n = sendto(sock, "Got your message\n", 17,
                   0, (struct sockaddr *)&from, fromlen);
        if (n < 0)
            error("sendto");
    }
    return 0;
}
