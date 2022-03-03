#include "customProtocol.h"

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

void timeout(void)
{
    int timeout_sec = 3;
    int timeout_milli = timeout_sec * 1000;
    // printf("Timeout: %d seconds\n", timeout_sec);
    int elapsed_time = clock() * 1000 / CLOCKS_PER_SEC;
    int end_time = elapsed_time + timeout_milli;
    while (elapsed_time <= end_time)
    {
        elapsed_time = clock() * 1000 / CLOCKS_PER_SEC;
    }
    // printf("Timeout reached\n");
}

// TODO: Add struct accessors:
void reset_data_packet(data_packet_t *packet, uint8_t client_id, uint8_t segment_no, uint8_t length, uint8_t *payload)
{
    //
}
void reset_ack_packet(ack_packet_t *packet, uint8_t client_id, uint8_t received_segment_no)
{
    //
}
void reset_reject_packet(reject_packet_t *packet, uint8_t client_id, REJECT_SUB_CODE sub_code, uint8_t received_segment_no)
{
    //
}

void update_data_packet(data_packet_t *packet, uint8_t client_id, uint8_t segment_no, uint8_t length, uint8_t *payload)
{
    //
}
void update_ack_packet(ack_packet_t *packet, uint8_t client_id, uint8_t received_segment_no)
{
    //
}
void update_reject_packet(reject_packet_t *packet, uint8_t client_id, REJECT_SUB_CODE sub_code, uint8_t received_segment_no)
{
    //
}

char *data_packet_to_string(data_packet_t *packet)
{
    // char string[347] = {}; // Size from compiler warning
    char *string = malloc(sizeof(char) * (DATA_PACKET_STRING_SIZE + 1));
    memset(string, 0, DATA_PACKET_STRING_SIZE + 1);
    snprintf(
        string,
        DATA_PACKET_STRING_SIZE,
        "\nstart=\t%04X\nclient_id=\t%hu\npacket_type=\t%04X\nsegment_no=\t%02X\nlength=\t%02X\npayload=\t%s\nend=\t%04X\n",
        packet->start_packet,
        packet->client_id,
        (uint16_t)packet->packet_type,
        packet->segment_no,
        packet->length,
        packet->payload,
        packet->end_packet);
    string[DATA_PACKET_STRING_SIZE] = '\0';
    return string;
}
char *ack_packet_to_string(ack_packet_t *packet)
{
    // char string[82] = {}; // Size from compiler warning
    char *string = malloc(sizeof(char) * (ACK_PACKET_STRING_SIZE + 1));
    memset(string, 0, ACK_PACKET_STRING_SIZE + 1);
    snprintf(
        string,
        ACK_PACKET_STRING_SIZE,
        "\nstart=\t%04X\nclient_id=\t%hu\npacket_type=\t%04X\nreceived_segment_no=\t%hu\nend=\t%04X\n",
        packet->start_packet,
        packet->client_id,
        (uint16_t)packet->packet_type,
        packet->received_segment_no,
        packet->end_packet);
    string[ACK_PACKET_STRING_SIZE] = '\0';
    return string;
}
char *reject_packet_to_string(reject_packet_t *packet)
{
    // char string[97] = {}; // Size from compiler warning
    char *string = malloc(sizeof(char) * (REJECT_PACKET_STRING_SIZE + 1));
    memset(string, 0, REJECT_PACKET_STRING_SIZE + 1);
    snprintf(
        string,
        REJECT_PACKET_STRING_SIZE,
        "\nstart=\t%04X\nclient_id=\t%hu\npacket_type=\t%04X\nsub_code=\t%04X\nreceived_segment_no=\t%hu\nend=\t%04X\n",
        packet->start_packet,
        packet->client_id,
        (uint16_t)packet->packet_type,
        (uint16_t)packet->sub_code,
        packet->received_segment_no,
        packet->end_packet);
    string[REJECT_PACKET_STRING_SIZE] = '\0';
    return string;
}
