#ifndef PKT_H
#define PKT_H

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

Packet create_packet(PacketHeader header, GroupIdentifier gid, OpcodeIdentifier oid, uint8_t *payload);

int send_packet(int tty_fd, Packet packet);

int rcv_packet(int tty_fd, Packet *packet);

void print_packet_header(PacketHeader header);

int *init_payload(uint8_t *buf, uint32_t sid);

int *create_payload(uint8_t *buf, uint32_t sid, uint8_t payload_extra[], uint8_t payload_extra_len);

int send_n_receive(int tty_fd, Packet packet, Packet *rcv_pkt);

#endif // PKT_H