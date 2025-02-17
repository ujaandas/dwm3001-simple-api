#include "board/commands.h"
#include "board/device_tags.h"

int *init_payload(uint8_t *buf, uint32_t sid)
{
  uint8_t init_payload[4];
  init_payload[0] = (uint8_t)(sid & 0xFF);
  init_payload[1] = (uint8_t)((sid >> 8) & 0xFF);
  init_payload[2] = (uint8_t)((sid >> 16) & 0xFF);
  init_payload[3] = (uint8_t)((sid >> 24) & 0xFF);

  return memcpy(buf, init_payload, 4);
}

int send_n_receive(int tty_fd, ControlPacket packet, ControlPacket *rcv_pkt)
{
  // Send the SESSION_INIT command
  printf("  cmd: Sending %s command...\n", oid_t_s(packet.gid, packet.oid));
  if (send_packet(tty_fd, packet) < 0)
  {
    return -1; // Failed to send command
  }

  sleep(1);

  // Receive the response
  *rcv_pkt = rcv_packet(tty_fd);
  if (rcv_pkt->gid != packet.gid || rcv_pkt->oid != packet.oid || rcv_pkt->payload[0] != STATUS_OK)
  {
    return -2; // Failed to receive valid response
  }

  return 1; // Success
}

int reset_device(int tty_fd)
{
  uint8_t temp_payload = REASON_RESET_POWER_CYCLE;
  ControlPacket send_pkt = create_packet(COMMAND, COMPLETE, CORE, CORE_DEVICE_RESET, &temp_payload, 1);
  ControlPacket rcv_pkt;
  return send_n_receive(tty_fd, send_pkt, &rcv_pkt);
}

int get_device_info(int tty_fd)
{
  // Create the CORE_GET_DEVICE_INFO packet
  ControlPacket send_pkt = create_packet(COMMAND, COMPLETE, CORE, CORE_GET_DEVICE_INFO, NULL, 0);
  ControlPacket rcv_pkt;

  uint8_t ret = send_n_receive(tty_fd, send_pkt, &rcv_pkt);
  if (!ret)
  {
    return ret;
  }

  // Parse device info
  int offset = 0;

  // NOTE: C for some reason does not like 2 operations in one, so for instance
  // uint16_t uci_ver = (rcv_pkt.payload[offset++] << 8) | rcv_pkt.payload[offset++];
  // is undefined behaviour. The existing solution is, IMO, much uglier,
  // but does avoid the warnings so whatever.

  uint8_t status = rcv_pkt.payload[offset++];

  uint16_t uci_ver = rcv_pkt.payload[offset] | (rcv_pkt.payload[offset + 1] << 8);
  offset += 2;

  uint16_t mac_ver = rcv_pkt.payload[offset] | (rcv_pkt.payload[offset + 1] << 8);
  offset += 2;

  uint16_t phy_ver = rcv_pkt.payload[offset] | (rcv_pkt.payload[offset + 1] << 8);
  offset += 2;

  uint16_t test_ver = rcv_pkt.payload[offset] | (rcv_pkt.payload[offset + 1] << 8);
  offset += 2;

  uint8_t v_info_len = rcv_pkt.payload[offset++];

  uint32_t device_id = rcv_pkt.payload[offset] | (rcv_pkt.payload[offset + 1] << 8) |
                       (rcv_pkt.payload[offset + 2] << 16) | (rcv_pkt.payload[offset + 3] << 24);
  offset += 4;

  uint32_t part_id = rcv_pkt.payload[offset] | (rcv_pkt.payload[offset + 1] << 8) |
                     (rcv_pkt.payload[offset + 2] << 16) | (rcv_pkt.payload[offset + 3] << 24);
  offset += 4;

  uint32_t firmware_ver = rcv_pkt.payload[offset] | (rcv_pkt.payload[offset + 1] << 8) |
                          (rcv_pkt.payload[offset + 2] << 16);
  offset += 3;

  uint8_t aoa_cap = rcv_pkt.payload[offset++];

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

  return 1; // Success
}

int init_uwb_session(int tty_fd, uint32_t sid, uint8_t stype)
{
  // Create the SESSION_INIT packet
  uint8_t payload[5];
  init_payload(payload, sid);
  payload[4] = stype;

  ControlPacket send_pkt = create_packet(COMMAND, COMPLETE, SESSION, SESSION_INIT, payload, 5);
  ControlPacket rcv_pkt;

  return send_n_receive(tty_fd, send_pkt, &rcv_pkt);
}

int set_uwb_controller(int tty_fd, uint32_t sid)
{
  uint8_t session_params[] = {
      DEVICE_TYPE, 0x01,              // DeviceType: Controller
      DEVICE_ROLE, 0x01,              // DeviceRole: Responder
      DEVICE_MAC_ADDRESS, 0x00, 0x00, // DeviceMacAddress: 0x0000
      NUMBER_OF_CONTROLEES, 0x01,     // NumberOfControlees: 1
      DST_MAC_ADDRESS, 0x01, 0x00,    // DstMacAddress: 0x0001
      // misc params 1
      RESULT_REPORT_CONFIG, 0x0B,                        // ResultReportConfig: 0x0B
      VENDOR_ID, 0x08, 0x07,                             // VendorId: 0x0708
      STATIC_STS_IV, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // StaticStsIv: 0x060504030201
      AOA_RESULT_REQ, 0x01,                              // AoaResultReq: 1
      UWB_INITIATION_TIME, 0xE8, 0x03, 0x00, 0x00,       // UwbInitiationTime: 1000
      RANGING_ROUND_USAGE, 0x02,                         // RangingRoundUsage: 2
      CHANNEL_NUMBER, 0x09,                              // ChannelNumber: 9
      PREAMBLE_CODE_INDEX, 0x09,                         // PreambleCodeIndex: 9
      // misc params 2
      RFRAME_CONFIG, 0x03,                      // RframeConfig: 3
      SFD_ID, 0x02,                             // SfdId: 2
      SLOT_DURATION, 0x60, 0x09,                // SlotDuration: 2400
      RANGING_INTERVAL, 0xC8, 0x00, 0x00, 0x00, // RangingInterval: 200
      SLOTS_PER_RR, 0x19,                       // SlotsPerRr: 25
      MULTI_NODE_MODE, 0x00,                    // MultiNodeMode: 0
      HOPPING_MODE, 0x00,                       // HoppingMode: 0
      RSSI_REPORTING, 0x01,                     // RssiReporting: 1
      ENABLE_DIAGNOSTICS, 0x01,                 // EnableDiagnostics: 1
      DIAGS_FRAME_REPORTS_FIELDS, 0x01          // DiagsFrameReportsFields: 1
  };
  uint8_t session_params_len = sizeof(session_params);
  return set_uwb_session_parameters(tty_fd, sid, session_params, session_params_len);
}

int set_uwb_controlee(int tty_fd, uint32_t sid)
{
  uint8_t session_params[] = {
      DEVICE_TYPE, 0x00,              // DeviceType: Controlee
      DEVICE_ROLE, 0x00,              // DeviceRole: Initiator
      DEVICE_MAC_ADDRESS, 0x01, 0x00, // DeviceMacAddress: 0x0001
      DST_MAC_ADDRESS, 0x00, 0x00,    // DstMacAddress: 0x0000
      // misc params 1
      RESULT_REPORT_CONFIG, 0x0B,                        // ResultReportConfig: 0x0B
      VENDOR_ID, 0x08, 0x07,                             // VendorId: 0x0708
      STATIC_STS_IV, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // StaticStsIv: 0x060504030201
      AOA_RESULT_REQ, 0x01,                              // AoaResultReq: 1
      UWB_INITIATION_TIME, 0xE8, 0x03, 0x00, 0x00,       // UwbInitiationTime: 1000
      RANGING_ROUND_USAGE, 0x02,                         // RangingRoundUsage: 2
      CHANNEL_NUMBER, 0x09,                              // ChannelNumber: 9
      PREAMBLE_CODE_INDEX, 0x09,                         // PreambleCodeIndex: 9
      // misc params 2
      RFRAME_CONFIG, 0x03,                      // RframeConfig: 3
      SFD_ID, 0x02,                             // SfdId: 2
      SLOT_DURATION, 0x60, 0x09,                // SlotDuration: 2400
      RANGING_INTERVAL, 0xC8, 0x00, 0x00, 0x00, // RangingInterval: 200
      SLOTS_PER_RR, 0x19,                       // SlotsPerRr: 25
      MULTI_NODE_MODE, 0x00,                    // MultiNodeMode: 0
      HOPPING_MODE, 0x00,                       // HoppingMode: 0
      RSSI_REPORTING, 0x01,                     // RssiReporting: 1
      ENABLE_DIAGNOSTICS, 0x01,                 // EnableDiagnostics: 1
      DIAGS_FRAME_REPORTS_FIELDS, 0x01          // DiagsFrameReportsFields: 1
  };
  uint8_t session_params_len = sizeof(session_params);
  return set_uwb_session_parameters(tty_fd, sid, session_params, session_params_len);
}

int set_uwb_session_parameters(int tty_fd, uint32_t sid, uint8_t session_params[], uint8_t session_params_len)
{
  uint8_t payload[MAX_PAYLOAD_SIZE];
  init_payload(payload, sid);

  int offset;
  for (offset = 4; offset < session_params_len; offset++)
  {
    payload[offset] = session_params[offset];
  }

  ControlPacket send_pkt = create_packet(COMMAND, COMPLETE, SESSION, SESSION_SET_APP_CONFIG, payload, offset);
  ControlPacket rcv_pkt;

  return send_n_receive(tty_fd, send_pkt, &rcv_pkt);
}

int start_uwb_ranging_session(int tty_fd, uint32_t sid)
{
  uint8_t payload[4];
  init_payload(payload, sid);

  ControlPacket send_pkt = create_packet(COMMAND, COMPLETE, RANGING, RANGE_START, payload, 4);
  ControlPacket rcv_pkt;

  return send_n_receive(tty_fd, send_pkt, &rcv_pkt);
}

int stop_uwb_ranging_session(int tty_fd, uint32_t sid)
{
  uint8_t payload[4];
  init_payload(payload, sid);

  ControlPacket send_pkt = create_packet(COMMAND, COMPLETE, RANGING, RANGE_STOP, payload, 4);
  ControlPacket rcv_pkt;

  return send_n_receive(tty_fd, send_pkt, &rcv_pkt);
}

int deinit_uwb_session(int tty_fd, uint32_t sid)
{
  uint8_t payload[4];
  init_payload(payload, sid);

  ControlPacket send_pkt = create_packet(COMMAND, COMPLETE, SESSION, SESSION_DEINIT, payload, 4);
  ControlPacket rcv_pkt;
  return send_n_receive(tty_fd, send_pkt, &rcv_pkt);
}

void receive_process_notif(int tty_fd)
{
  while (1) // Loop to continuously receive and process notifications
  {
    // sleep(2);
    printf("\n\n\n");
    ControlPacket rcv_pkt = rcv_packet(tty_fd);
    print_packet_header(rcv_pkt.header);
    printf("    pkt: GID: %s (0x%04X)\n", gid_t_s(rcv_pkt.gid), rcv_pkt.gid);
    printf("    pkt: OID: %s (0x%04X)\n", oid_t_s(rcv_pkt.gid, rcv_pkt.oid), rcv_pkt.oid);

    // Check if the packet is a ranging data notification
    if (rcv_pkt.gid && rcv_pkt.oid)
    {
      int offset = 0;
      // int sequence_number = *(int *)(buffer + offset);
      offset += 4;
      // int session_id = *(int *)(buffer + offset);
      offset += 4;
      // int rcr_indication = *(buffer + offset);
      offset += 1;
      // int current_ranging_interval = *(int *)(buffer + offset);
      offset += 4;
      // int ranging_measurement_type = *(buffer + offset);
      offset += 1;
      // Skipping RFU bytes and reading Number of Ranging Measurements
      offset += 1 + 1 + 8;
      int number_of_ranging_measurements = *(rcv_pkt.payload + offset);
      offset += 1;

      for (int i = 0; i < number_of_ranging_measurements; i++)
      {
        int mac_address = *(int *)(rcv_pkt.payload + offset);
        offset += 2;
        int status = *(rcv_pkt.payload + offset);
        offset += 1;
        int distance = *(int *)(rcv_pkt.payload + offset);
        offset += 2;

        // Log or process the ranging measurement
        printf("MAC Address: %04X, Status: %d, Distance: %d\n",
               mac_address, status, distance);
      }
    }
  }
}

void gtfo(int tty_fd, uint32_t sid)
{
  printf("  cmd: OH SHIT OH SHIT OH SHIT\n");
  stop_uwb_ranging_session(tty_fd, sid);
  deinit_uwb_session(tty_fd, sid);
  sleep(1);
}