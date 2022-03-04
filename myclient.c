/**
 * @file myclient.c
 * @author Benjamin Wang (bwang4@scu.edu, ID: 1179478)
 * @brief Client using customized protocol on top of UDP protocol for sending
 * information to the server. Client side implementation of UDP client-server.
 * @version 0.1
 * @date 2022-03-02
 *
 * @copyright Copyright (c) 2022
 * @source: https://www.linuxhowtos.org/C_C++/socket.htm
 * @source: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 *
 */

#include "customProtocol.h"

/**
 * @brief Main function (Driver code)
 *
 * @param argc number of arguments
 * @param argv arguments
 * @return int 0 if successful
 */
int main(int argc, char *argv[])
{
    // Local variables
    int sock, n, port;
    struct sockaddr_in server, client;
    unsigned int length = sizeof(struct sockaddr_in);
    struct hostent *hp;
    char buffer[sizeof(data_packet_t)];

    // Default port number and hostname
    char *host = HOSTNAME;
    port = PORT;

    // Checking if usage is correct
    if (argc != 2)
    {
        printf("Usage: input_file\n");
        exit(EXIT_FAILURE);
    }

    // Reading input file:

    // Create socket:
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        error("Error: socket");

    // Filling server information
    server.sin_family = AF_INET;
    if ((hp = gethostbyname((const char *)host)) == 0)
        error("Error: Unknown host");
    bcopy((char *)hp->h_addr,
          (char *)&server.sin_addr,
          hp->h_length);
    server.sin_port = htons(port);
    // server.sin_addr = htons(PORT)

    // Getting the message from the user
    // TODO: Replace with getline() from input files
    printf("Please enter the message: ");
    bzero(buffer, LINE_LENGTH);
    fgets(buffer, LINE_LENGTH - 1, stdin);

    uint8_t client_id = 123, seg_no = 0, buffLen = strlen(buffer);
    data_packet_t data_packet = {};
    size_t data_packet_size = sizeof(data_packet);
    reset_data_packet(&data_packet);
    update_data_packet(&data_packet, client_id, seg_no, buffLen, buffer);

    // Send message to server
    n = sendto(sock, (const data_packet_t *)&data_packet,
               data_packet_size, 0, (const struct sockaddr *)&server, length);
    if (n < 0)
        error("Error: Sendto");

    // Receiving the response:
    bzero(buffer, LINE_LENGTH); // Sanitize buffer
    n = recvfrom(sock, buffer, LINE_LENGTH, 0, (struct sockaddr *)&client, &length);
    if (n < 0)
        error("Error: recvfrom");
    buffer[n] = '\0';
    printf("Got response from server: %s\n", buffer);

    // Close the socket (Housekeeping)
    close(sock);
    return EXIT_SUCCESS;
}
