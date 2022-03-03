/**
 * @file customProtocol.h
 * @author Benjamin Wang (bwang4@scu.edu, ID: 1179478)
 * @brief Client using customized protocol on top of UDP protocol for sending
 * information to the server. Header file implementing the CUSTOM PROTOCOL
 * @date 2022-03-02
 *
 * @copyright Copyright (c) 2022
 *
 */

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
    printf("Timeout: %d seconds\n", timeout_sec);
    int elapsed_time = clock() * 1000 / CLOCKS_PER_SEC;
    int end_time = elapsed_time + timeout_milli;
    while (elapsed_time <= end_time)
    {
        elapsed_time = clock() * 1000 / CLOCKS_PER_SEC;
    }
    printf("Timeout reached\n");
}

bool is_valid_data_packet(data_packet_t *packet)
{
    if (packet->start_packet != START_PACKET)
    {
        printf("Error: Invalid start packet\n");
        return false;
    }
    if (packet->packet_type != PACKET_DATA)
    {
        printf("Error: Invalid packet type 0x%04X\n", packet->packet_type);
        return false;
    }
    if (packet->segment_no >= PACKET_GROUP_SIZE) // [0 - (size-1)]
    {
        printf("Error: Invalid segment number %hu\n", packet->segment_no);
        return false;
    }
    if (packet->end_packet != END_PACKET)
    {
        printf("Error: Invalid end packet\n");
        return false;
    }
    return true;
}

bool is_valid_ack_packet(ack_packet_t *packet)
{
    if (packet->start_packet != START_PACKET)
    {
        printf("Error: Invalid start packet\n");
        return false;
    }
    if (packet->packet_type != PACKET_ACK)
    {
        printf("Error: Invalid packet type 0x%04X\n", packet->packet_type);
        return false;
    }
    if (packet->received_segment_no >= PACKET_GROUP_SIZE) // [0 - (size-1)]
    {
        printf("Error: Invalid received segment number %hu\n", packet->received_segment_no);
        return false;
    }
    if (packet->end_packet != END_PACKET)
    {
        printf("Error: Invalid end packet\n");
        return false;
    }
    return true;
}

bool is_valid_reject_packet(reject_packet_t *packet)
{
    if (packet->start_packet != START_PACKET)
    {
        printf("Error: Invalid start packet 0x%04X\n", packet->start_packet);
        return false;
    }
    if (packet->packet_type != PACKET_REJECT)
    {
        printf("Error: Invalid packet type 0x%04X\n", packet->packet_type);
        return false;
    }
    if (packet->sub_code < REJECT_OUT_OF_SEQUENCE ||
        packet->sub_code > REJECT_DUPLICATE_PACKET)
    {
        printf("Error: Invalid reject sub code 0x%04X\n", packet->sub_code);
        return false;
    }
    if (packet->received_segment_no >= PACKET_GROUP_SIZE) // [0 - (size-1)]
    {
        printf("Error: Invalid received segment number %hu\n", packet->received_segment_no);
        return false;
    }
    if (packet->end_packet != END_PACKET)
    {
        printf("Error: Invalid end packet 0x%04X\n", packet->end_packet);
        return false;
    }
    return true;
}

void reset_data_packet(data_packet_t *packet)
{
    packet->start_packet = START_PACKET;
    packet->client_id = DEFAULT_VALUE;
    packet->packet_type = PACKET_DATA;
    packet->segment_no = DEFAULT_VALUE;
    packet->length = DEFAULT_VALUE;
    memset(packet->payload, DEFAULT_VALUE, sizeof(PACKET_DATA_PAYLOAD_SIZE));
    packet->end_packet = END_PACKET;
}
void reset_ack_packet(ack_packet_t *packet)
{
    packet->start_packet = START_PACKET;
    packet->client_id = DEFAULT_VALUE;
    packet->packet_type = PACKET_ACK;
    packet->received_segment_no = DEFAULT_VALUE;
    packet->end_packet = END_PACKET;
}
void reset_reject_packet(reject_packet_t *packet)
{
    packet->start_packet = START_PACKET;
    packet->client_id = DEFAULT_VALUE;
    packet->packet_type = PACKET_REJECT;
    packet->sub_code = DEFAULT_VALUE;
    packet->received_segment_no = DEFAULT_VALUE;
    packet->end_packet = END_PACKET;
}

void update_data_packet(data_packet_t *packet, uint8_t client_id, uint8_t segment_no, uint8_t length, char *payload)
{
    packet->client_id = client_id;
    packet->segment_no = segment_no;
    packet->length = length;
#ifdef DEBUGGING
    printf("RECEIVED LENGTH: %d\n", length);
#endif
    memcpy(packet->payload, payload, length);
}
void update_ack_packet(ack_packet_t *packet, uint8_t client_id, uint8_t received_segment_no)
{
    packet->client_id = client_id;
    packet->received_segment_no = received_segment_no;
}
void update_reject_packet(reject_packet_t *packet, uint8_t client_id, REJECT_SUB_CODE sub_code, uint8_t received_segment_no)
{
    packet->client_id = client_id;
    packet->sub_code = sub_code;
    packet->received_segment_no = received_segment_no;
}

bool data_packet_equals(data_packet_t *packet1, data_packet_t *packet2)
{
    if (packet1->start_packet != packet2->start_packet)
        return false;
    if (packet1->client_id != packet2->client_id)
        return false;
    if (packet1->packet_type != packet2->packet_type)
        return false;
    if (packet1->segment_no != packet2->segment_no)
        return false;
    if (packet1->length != packet2->length)
        return false;
    if (strlen(packet1->payload) != strlen(packet2->payload))
        return false;
    if (strcmp(packet1->payload, packet2->payload) != 0)
        return false;
    if (packet1->end_packet != packet2->end_packet)
        return false;
    return true;
}

char *data_packet_to_string(data_packet_t *packet)
{
    // char string[347] = {}; // Size from compiler warning
    char *string = malloc(sizeof(char) * (DATA_PACKET_STRING_SIZE + 1));
    memset(string, DEFAULT_VALUE, DATA_PACKET_STRING_SIZE + 1);
    snprintf(
        string,
        DATA_PACKET_STRING_SIZE,
        "\nstart=\t0x%04X\nclient_id=\t%hu\npacket_type=\t0x%04X\nsegment_no=\t%hu\nlength=\t%hu\npayload=\t%s\nend=\t0x%04X\n",
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
    memset(string, DEFAULT_VALUE, ACK_PACKET_STRING_SIZE + 1);
    snprintf(
        string,
        ACK_PACKET_STRING_SIZE,
        "\nstart=\t0x%04X\nclient_id=\t%hu\npacket_type=\t0x%04X\nreceived_segment_no=\t%hu\nend=\t0x%04X\n",
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
    memset(string, DEFAULT_VALUE, REJECT_PACKET_STRING_SIZE + 1);
    snprintf(
        string,
        REJECT_PACKET_STRING_SIZE,
        "\nstart=\t0x%04X\nclient_id=\t%hu\npacket_type=\t0x%04X\nsub_code=\t%04X\nreceived_segment_no=\t%hu\nend=\t0x%04X\n",
        packet->start_packet,
        packet->client_id,
        (uint16_t)packet->packet_type,
        (uint16_t)packet->sub_code,
        packet->received_segment_no,
        packet->end_packet);
    string[REJECT_PACKET_STRING_SIZE] = '\0';
    return string;
}
