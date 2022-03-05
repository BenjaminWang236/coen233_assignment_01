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
    int sock, n, port, ack_timer, end_time, ack_timer_reset_count, seg_count;
    struct sockaddr_in server, recv_from;
    unsigned int length = sizeof(struct sockaddr_in);
    struct hostent *hp;
    char buffer[sizeof(data_packet_t)];
    uint8_t peek_buff[1];
    bool response_received = false;

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
    seg_count = 1;

    uint8_t client_id = 123, seg_no = 0, buffLen = strlen(buffer);
    data_packet_t data_packet = {};
    size_t data_packet_size = sizeof(data_packet);
    ack_packet_t ack_packet = {};
    size_t ack_packet_size = sizeof(ack_packet);
    reject_packet_t reject_packet = {};
    size_t reject_packet_size = sizeof(reject_packet);

    // Loop through all the segments:
    for (seg_no = 0; seg_no < seg_count; seg_no++)
    {
        response_received = false;
        // Retry up to 3 times, first time (0th) is the original packet
        for (ack_timer_reset_count = 0; ack_timer_reset_count <= ACK_TIMER_RETRY_COUNT && !response_received; ack_timer_reset_count++)
        {
#ifdef DEBUGGING
            printf("Current ACK retry count: %d\n", ack_timer_reset_count);
#endif
            if (ack_timer_reset_count == 0)
            {
                reset_data_packet(&data_packet);
                update_data_packet(&data_packet, client_id, seg_no, buffLen, buffer);
            }
            else
            {
                printf("Server does not respond. Retrying attempt %d\n", ack_timer_reset_count);
            }

            // Send message to server
            n = sendto(sock, (const data_packet_t *)&data_packet,
                       data_packet_size, 0, (const struct sockaddr *)&server, length);
            if (n < 0)
                error("Error: Sendto");

#ifdef DEBUGGING
            printf("Starting ACK timer\n");
#endif
            // Start and set ACK_Timer:
            ack_timer = clock() * 1000 / CLOCKS_PER_SEC;
            end_time = ack_timer + ACK_TIMER_WAIT_TIME_MS;

            // Waiting for response:
            while (ack_timer <= end_time)
            {
                // Update ACK_Timer:
                ack_timer = clock() * 1000 / CLOCKS_PER_SEC;

                printf("ack_timer diff %d ms\n", end_time - ack_timer);

                // Peek the response packet:
                // NOTE: Peek only works on non-UNIX systems, for example doesn't work on WSL-Ubuntu
                bzero(peek_buff, sizeof(peek_buff));
                n = recvfrom(sock, peek_buff, sizeof(peek_buff), MSG_PEEK | MSG_TRUNC, (struct sockaddr *)&recv_from, &length);
                // n = recvfrom(sock, peek_buff, sizeof(peek_buff), 0, (struct sockaddr *)&recv_from, &length);
                if (n < 0)
                    error("Error: peek Recvfrom");
#ifdef DEBUGGING
                printf("Peeked packet size: %d\n", n);
#endif

                // Processing the Response from Server:
                if (n == ack_packet_size)
                {
                    memset(&ack_packet, 0, ack_packet_size);
                    n = recvfrom(sock, &ack_packet, ack_packet_size, 0, (struct sockaddr *)&recv_from, &length);
                    if (n < 0)
                        error("Error: recvfrom");
                    response_received = true;
#ifdef DEBUGGING
                    // printf("Received ACK packet.\n");
                    if (is_valid_ack_packet(&ack_packet))
                        printf("Received valid ACK packet.\n");
                    else
                        printf("Received invalid ACK packet.\n");
                    char *ack_str = ack_packet_to_string(&ack_packet);
                    printf("Received ACK packet: %s", ack_str);
                    free(ack_str);
#endif
                    break;
                }
                else if (n == reject_packet_size)
                {
                    memset(&reject_packet, 0, reject_packet_size);
                    n = recvfrom(sock, &reject_packet, reject_packet_size, 0, (struct sockaddr *)&recv_from, &length);
                    if (n < 0)
                        error("Error: recvfrom");
                    response_received = true;
#ifdef DEBUGGING
                    // printf("Raw reject packet: 0x%40X\n", reject_packet);
                    if (is_valid_reject_packet(&reject_packet))
                        printf("REJ packet formatted properly!\n");
                    else
                        printf("ERROR: REJ packet formatted improperly!\n");
                    char *rej_str = reject_packet_to_string(&reject_packet);
                    printf("Received REJECT packet: %s\n", rej_str);
                    free(rej_str);

#endif
                    break;
                    // If rejected, move on to next packet to send
                }
                else
                {
                    // This resonse is not an ACK or REJECT packet, so should not count as received properly
                    char *errString = "";
                    sprintf(errString, "Error: Invalid packet size/type received: %d\n", n);
                    error(errString);
                }
            }
        }
    }

    // Close the socket (Housekeeping)
    close(sock);
    return EXIT_SUCCESS;
}
