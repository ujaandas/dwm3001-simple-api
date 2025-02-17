#ifndef TAGS_H
#define TAGS_H

#define TAG_DEVICE_TYPE 0x00
#define TAG_DEVICE_ROLE 0x11
#define TAG_DEVICE_MAC_ADDRESS 0x06
#define TAG_NUMBER_OF_CONTROLEES 0x05
#define TAG_DST_MAC_ADDRESS 0x07
#define TAG_RESULT_REPORT_CONFIG 0x2E
#define TAG_VENDOR_ID 0x27
#define TAG_STATIC_STS_IV 0x28
#define TAG_AOA_RESULT_REQ 0x0D
#define TAG_UWB_INITIATION_TIME 0x2B
#define TAG_RANGING_ROUND_USAGE 0x01
#define TAG_CHANNEL_NUMBER 0x04
#define TAG_PREAMBLE_CODE_INDEX 0x14
#define TAG_RFRAME_CONFIG 0x12
#define TAG_SFD_ID 0x15
#define TAG_SLOT_DURATION 0x08
#define TAG_RANGING_INTERVAL 0x09
#define TAG_SLOTS_PER_RR 0x1B
#define TAG_MULTI_NODE_MODE 0x03
#define TAG_HOPPING_MODE 0x2C
#define TAG_RSSI_REPORTING 0x36
#define TAG_ENABLE_DIAGNOSTICS 0xE8
#define TAG_DIAGS_FRAME_REPORTS_FIELDS 0xE9

const uint8_t MISC_PARAMS[] = {
    TAG_RESULT_REPORT_CONFIG, 0x0B,                        // ResultReportConfig: 0x0B
    TAG_VENDOR_ID, 0x08, 0x07,                             // VendorId: 0x0708
    TAG_STATIC_STS_IV, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // StaticStsIv: 0x060504030201
    TAG_AOA_RESULT_REQ, 0x01,                              // AoaResultReq: 1
    TAG_UWB_INITIATION_TIME, 0xE8, 0x03, 0x00, 0x00,       // UwbInitiationTime: 1000
    TAG_RANGING_ROUND_USAGE, 0x02,                         // RangingRoundUsage: 2
    TAG_CHANNEL_NUMBER, 0x09,                              // ChannelNumber: 9
    TAG_PREAMBLE_CODE_INDEX, 0x09,                         // PreambleCodeIndex: 9
    TAG_RFRAME_CONFIG, 0x03,                               // RframeConfig: 3
    TAG_SFD_ID, 0x02,                                      // SfdId: 2
    TAG_SLOT_DURATION, 0x60, 0x09,                         // SlotDuration: 2400
    TAG_RANGING_INTERVAL, 0xC8, 0x00, 0x00, 0x00,          // RangingInterval: 200
    TAG_SLOTS_PER_RR, 0x19,                                // SlotsPerRr: 25
    TAG_MULTI_NODE_MODE, 0x00,                             // MultiNodeMode: 0
    TAG_HOPPING_MODE, 0x00,                                // HoppingMode: 0
    TAG_RSSI_REPORTING, 0x01,                              // RssiReporting: 1
    TAG_ENABLE_DIAGNOSTICS, 0x01,                          // EnableDiagnostics: 1
    TAG_DIAGS_FRAME_REPORTS_FIELDS, 0x01                   // DiagsFrameReportsFields: 1
};

#endif // TAGS_H
