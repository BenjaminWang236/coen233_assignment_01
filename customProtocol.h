/**
 * @file customProtocol.h
 * @author Benjamin Wang (bwang4@scu.edu, ID: 1179478)
 * @brief Client using customized protocol on top of UDP protocol for sending
 * information to the server. Header file defining the CUSTOM PROTOCOL
 * @date 2022-03-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef CUSTOMPROTOCOL_H /* include guard */
#define CUSTOMPROTOCOL_H

// library includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// Port and Hostname:
#define PORT 8080
#define HOSTNAME "localhost"

// Custom Protocol Primitives
#define START_PACKET 0xFFFF
#define END_PACKET 0xFFFF
#define MAX_CLIENT_ID 0XFF
#define MAX_PACKET_SIZE 0xFF

// Custom Protocol Packet Types
typedef enum
{
    PACKET_DATA = 0xFFF1,
    PACKET_ACK,
    PACKET_REJECT
} PACKET_TYPE;
// Custom Protocol Reject Sub-Codes
typedef enum
{
    REJECT_OUT_OF_SEQUENCE = 0xFFF4,
    REJECT_LENGTH_MISMATCH,
    REJECT_END_OF_PACKET_MISSING,
    REJECT_DUPLICATE_PACKET
} REJECT_SUB_CODE;

// Custom Protocol Packet Structure Sizes
#define PACKET_DATA_PAYLOAD_SIZE 255

// Custom Protocol Meta-Settings
#define PACKET_GROUP_SIZE 5
#define ACK_TIMER_WAIT_TIME_MS 3000
#define ACK_TIMER_RETRY_COUNT 3
#define MAXLINE 1024
#define LINE_LENGTH 256

// Custom Protocol Packet Structures:
typedef struct
{
    uint16_t start_packet;
    uint8_t client_id;
    PACKET_TYPE packet_type;
    uint8_t segment_no;
    uint8_t length;
    uint8_t payload[PACKET_DATA_PAYLOAD_SIZE];
    uint16_t end_packet;
} data_packet_t; // Size 266
// } __attribute__((packed)) data_packet_t; // Size 266

typedef struct
{
    uint16_t start_packet;
    uint8_t client_id;
    PACKET_TYPE packet_type;
    uint8_t received_segment_no;
    uint16_t end_packet;
} __attribute__((packed)) ack_packet_t; // Size 8

typedef struct
{
    uint16_t start_packet;
    uint8_t client_id;
    PACKET_TYPE packet_type;
    REJECT_SUB_CODE sub_code;
    uint8_t received_segment_no;
    uint16_t end_packet;
} __attribute__((packed)) reject_packet_t; // Size 10

/**
 * @brief Error function
 *
 * @param msg Error message
 */
void error(const char *msg);

/**
 * @brief Timeout function, testing purposes only
 */
void timeout(void);

#endif