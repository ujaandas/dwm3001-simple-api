#include "board/commands.h"
#include "board/tags.h"

int init_payload_sid(uint8_t *payload, uint8_t payload_len, uint32_t sid)
{
  for (int i = 0; i < sizeof(uint32_t) / sizeof(payload[0]); i++)
  {
    payload[i] = (uint8_t)((sid >> i * 8) & 0xFF);
  }
  return 0;
}

int encode_payload(uint8_t *payload, uint8_t payload_len, uint8_t *data, uint8_t data_len)
{
  if (data_len > payload_len)
  {
    return -1;
  }

  for (int i = 0; i < data_len && payload[i] == 0; i++)
  {
    payload[i] = data[i];
  }

  return 0;
}

int send_n_receive(int tty_fd, Packet sent_pkt, Packet *rcvd_pkt)
{
  printf("  cmd: Sending %s command...\n", oid_t_s(sent_pkt.gid, sent_pkt.oid));
  if (send_packet(tty_fd, sent_pkt) < 0)
  {
    return -1;
  }

  rcv_packet(tty_fd, rcvd_pkt);
  if (rcvd_pkt->gid != sent_pkt.gid || rcvd_pkt->oid != sent_pkt.oid || rcvd_pkt->payload[0] != STATUS_OK)
  {
    return -2;
  }

  return 0;
}

int reset_device(int tty_fd)
{
  uint8_t temp_payload = REASON_RESET_POWER_CYCLE;
  Packet sent_pkt = create_packet(CommandHeader, CORE, CORE_DEVICE_RESET, &temp_payload, 1);
  Packet rcvd_pkt;

  return send_n_receive(tty_fd, sent_pkt, &rcvd_pkt);
}

int get_device_info(int tty_fd)
{
  // Create the CORE_GET_DEVICE_INFO packet
  Packet sent_pkt = create_packet(CommandHeader, CORE, CORE_GET_DEVICE_INFO, NULL, 0);
  Packet rcvd_pkt;

  send_n_receive(tty_fd, sent_pkt, &rcvd_pkt);

  // Parse device info
  int offset = 0;

  // NOTE: C for some reason does not like 2 operations in one, so for instance
  // uint16_t uci_ver = (rcvd_pkt.payload[offset++] << 8) | rcvd_pkt.payload[offset++];
  // is undefined behaviour. The existing solution is, IMO, much uglier,
  // but does avoid the warnings so whatever.

  uint8_t status = rcvd_pkt.payload[offset++];

  uint16_t uci_ver = rcvd_pkt.payload[offset] | (rcvd_pkt.payload[offset + 1] << 8);
  offset += 2;

  uint16_t mac_ver = rcvd_pkt.payload[offset] | (rcvd_pkt.payload[offset + 1] << 8);
  offset += 2;

  uint16_t phy_ver = rcvd_pkt.payload[offset] | (rcvd_pkt.payload[offset + 1] << 8);
  offset += 2;

  uint16_t test_ver = rcvd_pkt.payload[offset] | (rcvd_pkt.payload[offset + 1] << 8);
  offset += 2;

  uint8_t v_info_len = rcvd_pkt.payload[offset++];

  uint32_t device_id = rcvd_pkt.payload[offset] | (rcvd_pkt.payload[offset + 1] << 8) |
                       (rcvd_pkt.payload[offset + 2] << 16) | (rcvd_pkt.payload[offset + 3] << 24);
  offset += 4;

  uint32_t part_id = rcvd_pkt.payload[offset] | (rcvd_pkt.payload[offset + 1] << 8) |
                     (rcvd_pkt.payload[offset + 2] << 16) | (rcvd_pkt.payload[offset + 3] << 24);
  offset += 4;

  uint32_t firmware_ver = rcvd_pkt.payload[offset] | (rcvd_pkt.payload[offset + 1] << 8) |
                          (rcvd_pkt.payload[offset + 2] << 16);
  offset += 3;

  uint8_t aoa_cap = rcvd_pkt.payload[offset++];

  // print all info
  printf("  cmd: Status: %2sx\n", status_to_s(status));
  printf("  cmd: UCI Ver: 0x%04X (%d)\n", uci_ver, uci_ver);
  printf("  cmd: MAC Ver: 0x%04X (%d)\n", mac_ver, mac_ver);
  printf("  cmd: Physical Ver: 0x%04X (%d)\n", phy_ver, phy_ver);
  printf("  cmd: Test Ver: 0x%04X (%d)\n", test_ver, test_ver);
  printf("  cmd: Vendor Information Length: 0x%04X\n", v_info_len);
  printf("  cmd: Device ID: 0x%08X (%d)\n", device_id, device_id);
  printf("  cmd: Part ID: 0x%08X (%d)\n", part_id, part_id);
  printf("  cmd: Firmware Ver: 0x%06X (%d)\n", firmware_ver, firmware_ver);
  printf("  cmd: AOA Capability: %s\n", aoa_cap == 0 ? "Error" : aoa_cap == 1 ? "True"
                                                                              : "False");

  return 0; // Success
}

int init_uwb_session(int tty_fd, uint32_t sid, uint8_t stype)
{
  // Create the SESSION_INIT packet
  uint8_t payload[5];
  init_payload_sid(payload, 5, sid);
  payload[4] = stype;

  Packet sent_pkt = create_packet(CommandHeader, SESSION, SESSION_INIT, payload, sizeof(payload));
  Packet rcvd_pkt;

  return send_n_receive(tty_fd, sent_pkt, &rcvd_pkt);
}

int set_uwb_controller(int tty_fd, uint32_t sid)
{
  uint8_t session_params[] = {
      TAG_DEVICE_TYPE, 0x01,              // DeviceType: Controller
      TAG_DEVICE_ROLE, 0x01,              // DeviceRole: Responder
      TAG_DEVICE_MAC_ADDRESS, 0x00, 0x00, // DeviceMacAddress: 0x0000
      TAG_NUMBER_OF_CONTROLEES, 0x01,     // NumberOfControlees: 1
      TAG_DST_MAC_ADDRESS, 0x01, 0x00     // DstMacAddress: 0x0001
  };

  uint8_t session_params_len = sizeof(session_params);
  uint8_t total_params_len = session_params_len + sizeof(MISC_PARAMS);

  uint8_t complete_params[total_params_len];
  memcpy(complete_params, session_params, session_params_len);
  memcpy(complete_params + session_params_len, MISC_PARAMS, sizeof(MISC_PARAMS));

  return set_uwb_session_parameters(tty_fd, sid, complete_params, total_params_len);
}

int set_uwb_controlee(int tty_fd, uint32_t sid)
{
  uint8_t session_params[] = {
      TAG_DEVICE_TYPE, 0x00,              // DeviceType: Controlee
      TAG_DEVICE_ROLE, 0x00,              // DeviceRole: Initiator
      TAG_DEVICE_MAC_ADDRESS, 0x01, 0x00, // DeviceMacAddress: 0x0001
      TAG_DST_MAC_ADDRESS, 0x00, 0x00     // DstMacAddress: 0x0000
  };

  uint8_t session_params_len = sizeof(session_params);
  uint8_t total_params_len = session_params_len + sizeof(MISC_PARAMS);

  uint8_t complete_params[total_params_len];
  memcpy(complete_params, session_params, session_params_len);
  memcpy(complete_params + session_params_len, MISC_PARAMS, sizeof(MISC_PARAMS));

  return set_uwb_session_parameters(tty_fd, sid, complete_params, total_params_len);
}

int set_uwb_session_parameters(int tty_fd, uint32_t sid, uint8_t session_params[], uint8_t session_params_len)
{
  uint8_t payload[MAX_PAYLOAD_SIZE];
  int offset = 0;

  payload[offset++] = (uint8_t)(sid & 0xFF);
  payload[offset++] = (uint8_t)((sid >> 8) & 0xFF);
  payload[offset++] = (uint8_t)((sid >> 16) & 0xFF);
  payload[offset++] = (uint8_t)((sid >> 24) & 0xFF);

  for (int i = 0; i < session_params_len; i++)
  {
    payload[offset++] = session_params[i];
  }

  Packet sent_pkt = create_packet(CommandHeader, SESSION, SESSION_SET_APP_CONFIG, payload, offset);
  Packet rcvd_pkt;

  return send_n_receive(tty_fd, sent_pkt, &rcvd_pkt);
}

int start_uwb_ranging_session(int tty_fd, uint32_t sid)
{
  uint8_t payload[4];
  payload[0] = (uint8_t)(sid & 0xFF);
  payload[1] = (uint8_t)((sid >> 8) & 0xFF);
  payload[2] = (uint8_t)((sid >> 16) & 0xFF);
  payload[3] = (uint8_t)((sid >> 24) & 0xFF);

  Packet sent_pkt = create_packet(CommandHeader, RANGING, RANGE_START, payload, 4);
  Packet rcvd_pkt;

  return send_n_receive(tty_fd, sent_pkt, &rcvd_pkt);
}

int stop_uwb_ranging_session(int tty_fd, uint32_t sid)
{
  uint8_t payload[4];
  payload[0] = (uint8_t)(sid & 0xFF);
  payload[1] = (uint8_t)((sid >> 8) & 0xFF);
  payload[2] = (uint8_t)((sid >> 16) & 0xFF);
  payload[3] = (uint8_t)((sid >> 24) & 0xFF);

  Packet sent_pkt = create_packet(CommandHeader, RANGING, RANGE_STOP, payload, 4);
  Packet rcvd_pkt;

  return send_n_receive(tty_fd, sent_pkt, &rcvd_pkt);
}

int deinit_uwb_session(int tty_fd, uint32_t sid)
{
  uint8_t payload[4];
  payload[0] = (uint8_t)(sid & 0xFF);
  payload[1] = (uint8_t)((sid >> 8) & 0xFF);
  payload[2] = (uint8_t)((sid >> 16) & 0xFF);
  payload[3] = (uint8_t)((sid >> 24) & 0xFF);

  Packet sent_pkt = create_packet(CommandHeader, SESSION, SESSION_DEINIT, payload, 4);
  Packet rcvd_pkt;

  return send_n_receive(tty_fd, sent_pkt, &rcvd_pkt);
}

void receive_process_notif(int tty_fd)
{
  while (1) // Loop to continuously receive and process notifications
  {
    sleep(1);
    printf("\n\n\n");
    Packet rcvd_packet;
    rcv_packet(tty_fd, &rcvd_packet);
    print_packet_header(rcvd_packet.header);
    printf("    pkt: GID: %s (0x%04X)\n", gid_t_s(rcvd_packet.gid), rcvd_packet.gid);
    printf("    pkt: OID: %s (0x%04X)\n", oid_t_s(rcvd_packet.gid, rcvd_packet.oid), rcvd_packet.oid);
  }
}

void gtfo(int tty_fd, uint32_t sid)
{
  printf("  cmd: OH SHIT OH SHIT OH SHIT\n");
  stop_uwb_ranging_session(tty_fd, sid);
  deinit_uwb_session(tty_fd, sid);
  sleep(1);
}