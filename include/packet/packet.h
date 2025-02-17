#ifndef CTRL_PKT_H
#define CTRL_PKT_H

#include <string.h>
#include "header/header.h"
#include "identifiers/gid.h"
#include "identifiers/oid.h"
#include "identifiers/status.h"
#include "transport/tty.h"

#define MAX_PAYLOAD_SIZE 255

typedef struct
{
  PacketHeader header;
  GroupIdentifier gid;
  OpcodeIdentifier oid;
  uint8_t payload_len;
  uint8_t payload[MAX_PAYLOAD_SIZE];
} Packet;

void print_packet_header(PacketHeader header);

Packet create_packet(PacketHeader header, GroupIdentifier gid, OpcodeIdentifier oid, uint8_t *payload, uint8_t payload_len);

// Function to send a ControlPackets
int send_packet(int tty_fd, Packet packet);

Packet rcv_packet(int tty_fd);

#endif // CTRL_PKT_H