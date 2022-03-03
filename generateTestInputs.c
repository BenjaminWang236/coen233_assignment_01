/**
 * @file generateTestInputs.c
 * @author Benjamin Wang (bwang4@scu.edu, ID: 1179478)
 * @brief Client using customized protocol on top of UDP protocol for sending
 * information to the server. Generate the test cases as input files, and also debugging
 * @version 0.1
 * @date 2022-03-02
 *
 * @copyright Copyright (c) 2022
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
    // Define variables

#ifdef DEBUGGING
    printf("Test: Rej Dup Packet:\t0x%04X\n", REJECT_DUPLICATE_PACKET);
    printf("Test: hostname: %s\n", HOSTNAME);
    printf("Test: data_packet_t size: %i\n", (int)sizeof(data_packet_t));
    printf("Test: ack_packet_t size: %i\n", (int)sizeof(ack_packet_t));
    printf("Test: reject_packet_t size: %i\n", (int)sizeof(reject_packet_t));
    // timeout();
    char *hello = "Hello World!";
    data_packet_t data_packet = {};
    reset_data_packet(&data_packet);
    update_data_packet(&data_packet, 255, 5, strlen(hello), hello);
    if ((is_valid_data_packet(&data_packet)))
        printf("Test: data_packet is valid\n");
    else
        printf("Test: data_packet is invalid\n");
    // printf("payload: %s\n", data_packet.payload);
    printf("data_packet_to_string: %s", data_packet_to_string(&data_packet));
    printf("payload length: strlen: %lu\tsizeof: %lu\n", strlen(data_packet.payload), sizeof(data_packet.payload));
#endif

    return EXIT_SUCCESS;
}
