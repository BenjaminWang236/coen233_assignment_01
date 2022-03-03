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
 * @source: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

/**
 * @brief Error function
 *
 * @param msg Error message
 */
void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Main function (Driver code)
 *
 * @param argc number of arguments
 * @param argv arguments
 * @return int 0 if successful
 */
int main(int argc, char *argv[])
{
    // Define variables
    int sock, length, n;
    socklen_t clientlen;
    struct sockaddr_in server, client;
    char buf[MAXLINE];
    char *hello = "Hello from server";

    // Checking if port number is provided
    if (argc < 2)
    {
        fprintf(stderr, "ERROR: no port provided\n");
        exit(EXIT_FAILURE);
    }

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        error("ERROR: Opening socket");

    // Filling server information
    length = sizeof(server);
    bzero(&server, length); // memset(&servaddr, 0, length);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));
    // server.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sock, (struct sockaddr *)&server, length) < 0)
        error("ERROR: binding");

    // Initializing client
    clientlen = sizeof(client);
    bzero(&client, clientlen);

    // Server runs forever, I guess
    while (1)
    {
        n = recvfrom(sock, buf, MAXLINE, 0, (struct sockaddr *)&client, &clientlen);
        if (n < 0)
            error("ERROR: recvfrom");
        buf[n] = '\0';
        printf("Received a datagram: %s\n", buf);

        // ACK or REJ logic here:
        // TODO: Check for all 4 error-cases!

        // Sending response back to Client
        // TODO: Change MSG_CONFIRM to to custom message
        n = sendto(sock, (const char *)hello, strlen(hello),
                   MSG_CONFIRM, (struct sockaddr *)&client, clientlen);
        if (n < 0)
            error("ERROR: sendto");
    }
    return EXIT_SUCCESS;
}
