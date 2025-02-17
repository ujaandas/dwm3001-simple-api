#include "packet/packet.h"

void print_packet_header(PacketHeader header)
{
  printf("    pkt: Message Type: %s\n", mt_t_s(header.mt));
  printf("    pkt: Packet Boundary Flag: %s\n", pbf_t_s(header.pbf));
}

Packet create_packet(PacketHeader header, GroupIdentifier gid, OpcodeIdentifier oid, uint8_t *payload, uint8_t payload_len)
{
  Packet packet;
  packet.header = header;
  packet.gid = gid;
  packet.oid = oid;
  packet.payload_len = payload_len;
  memcpy(packet.payload, payload, payload_len);
  return packet;
}

// Function to send a ControlPackets
int send_packet(int tty_fd, Packet packet)
{
  printf("    pkt: Sending packet...\n");
  uint8_t buffer[4 + packet.payload_len];

  // Octet 0: first 3 bits are mt, next bit is pbf, next 4 are gid
  buffer[0] = ((packet.header.mt & 0b00000111) << 5) |  // mask and shift the first 3 bits of mt
              ((packet.header.pbf & 0b00000001) << 4) | // mask and shift the pbf bit
              (packet.gid & 0b00001111);                // mask the gid bits

  // Octet 1: first 2 bits are rfu, next 6 bits are oid
  buffer[1] = (packet.oid & 0b00111111); // mask the oid bits

  // Octet 2: rfu
  buffer[2] = 0b00000000; // reserved for future use

  // Octet 3: payload length
  buffer[3] = packet.payload_len;

  // Octet 4 to 4 + payload length: payload
  memcpy(buffer + 4, packet.payload, packet.payload_len);

  return tty_send(tty_fd, buffer, sizeof(buffer));
}

int rcv_packet(int tty_fd, Packet *rcvd_pkt)
{
  uint8_t buffer[MAX_PAYLOAD_SIZE + 4];

  printf("    pkt: Receiving response...\n");
  int bytes_read = tty_rcv(tty_fd, buffer, sizeof(buffer));
  if (bytes_read < 4)
  {
    printf("    pkt: Failed to receive a valid response header\n");
    return -1;
  }

  // Print the header
  printf("    pkt: Received Header: ");
  for (int i = 0; i < 4; i++)
  {
    printf("%02X ", buffer[i]);
  }
  printf("\n");

  // Translate into Packet
  rcvd_pkt->header.mt = (buffer[0] >> 5) & 0b00000111;
  rcvd_pkt->header.pbf = (buffer[0] >> 4) & 0b00000001;
  rcvd_pkt->gid = (buffer[0] & 0b00001111);
  rcvd_pkt->oid = (buffer[1] & 0b00111111);
  rcvd_pkt->payload_len = buffer[3];

  print_packet_header(rcvd_pkt->header);

  // Print the payload
  printf("    pkt: Received Payload:\n");
  for (int i = 4; i < 4 + rcvd_pkt->payload_len; i++)
  {
    printf("    pkt: %02X (status: %s)\n", buffer[i], status_to_s(buffer[i]));
  }
  printf("    pkt: of length %d byte", rcvd_pkt->payload_len);
  printf("\n");

  // Copy the payload
  memcpy(rcvd_pkt->payload, buffer + 4, rcvd_pkt->payload_len);

  return 1;
}