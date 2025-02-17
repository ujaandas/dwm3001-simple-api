#ifndef DEVICE_TAGS_H
#define DEVICE_TAGS_H

#define DEVICE_TYPE 0x00
#define DEVICE_ROLE 0x11
#define DEVICE_MAC_ADDRESS 0x06
#define NUMBER_OF_CONTROLEES 0x05
#define DST_MAC_ADDRESS 0x07
#define RESULT_REPORT_CONFIG 0x2E
#define VENDOR_ID 0x27
#define STATIC_STS_IV 0x28
#define AOA_RESULT_REQ 0x0D
#define UWB_INITIATION_TIME 0x2B
#define RANGING_ROUND_USAGE 0x01
#define CHANNEL_NUMBER 0x04
#define PREAMBLE_CODE_INDEX 0x14
#define RFRAME_CONFIG 0x12
#define SFD_ID 0x15
#define SLOT_DURATION 0x08
#define RANGING_INTERVAL 0x09
#define SLOTS_PER_RR 0x1B
#define MULTI_NODE_MODE 0x03
#define HOPPING_MODE 0x2C
#define RSSI_REPORTING 0x36
#define ENABLE_DIAGNOSTICS 0xE8
#define DIAGS_FRAME_REPORTS_FIELDS 0xE9

uint8_t session_params_misc[] = {
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

#endif // DEVICE_TAGS_H
