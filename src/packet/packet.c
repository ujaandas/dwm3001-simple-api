#include "packet/packet.h"

void print_packet_header(PacketHeader header)
{
  printf("    pkt: Message Type: %s\n", mt_t_s(header.mt));
  printf("    pkt: Packet Boundary Flag: %s\n", pbf_t_s(header.pbf));
}

Packet create_packet(PacketHeader header, GroupIdentifier gid, OpcodeIdentifier oid, uint8_t *payload)
{
  Packet packet;
  packet.header = header;
  packet.gid = gid;
  packet.oid = oid;
  packet.payload_len = payload == NULL ? 0 : sizeof(payload);
  memcpy(packet.payload, payload, packet.payload_len);
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

int rcv_packet(int tty_fd, Packet *packet)
{
  uint8_t buffer[MAX_PAYLOAD_SIZE + 4];

  printf("    pkt: Receiving packet...\n");
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

  // Translate into ControlPacket
  packet->header.mt = (buffer[0] >> 5) & 0b00000111;
  packet->header.pbf = (buffer[0] >> 4) & 0b00000001;
  packet->gid = (buffer[0] & 0b00001111);
  packet->oid = (buffer[1] & 0b00111111);
  packet->payload_len = buffer[3];

  print_packet_header(packet->header);

  // Print the payload
  printf("    pkt: Received Payload:\n");
  for (int i = 4; i < 4 + packet->payload_len; i++)
  {
    printf("    pkt: %02X (status: %s)\n", buffer[i], status_to_s(buffer[i]));
  }
  printf("    pkt: of length %d byte", packet->payload_len);
  printf("\n");

  // Copy the payload
  memcpy(packet->payload, buffer + 4, packet->payload_len);

  return bytes_read;
}

int *init_payload(uint8_t *buf, uint32_t sid)
{
  uint8_t init_payload[4];
  for (int i = 0; i < 4; i++)
  {
    init_payload[i] = (uint8_t)((sid >> 8 * i) & 0xFF);
    // shift by 0, then 8, then 16... and mask to append
  }

  return memcpy(buf, init_payload, 4);
}

int *create_payload(uint8_t *buf, uint32_t sid, uint8_t payload_extra[], uint8_t payload_extra_len)
{
  int *ret = init_payload(buf, sid);
  if (!ret)
  {
    return ret;
  }
  return memcpy(buf, payload_extra + 4, payload_extra_len);
}

int send_n_receive(int tty_fd, Packet packet, Packet *rcv_pkt)
{
  // Send the SESSION_INIT command
  printf("  cmd: Sending %s command...\n", oid_t_s(packet.gid, packet.oid));
  if (send_packet(tty_fd, packet) < 0)
  {
    return -1; // Failed to send command
  }

  sleep(1);

  // Receive the response
  if (rcv_packet(tty_fd, rcv_pkt) < 0)
  {
    return -2;
  }
  if (rcv_pkt->gid != packet.gid || rcv_pkt->oid != packet.oid || rcv_pkt->payload[0] != STATUS_OK)
  {
    return -3; // Failed to receive valid response
  }

  return 1; // Success
}