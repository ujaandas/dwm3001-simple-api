#ifndef NOTIF_H
#define NOTIF_H

#include <stdint.h>
#include <string.h>
#include "header/header.h"
#include "identifiers/gid.h"
#include "identifiers/oid.h"

#define MIN_PAYLOAD_SIZE 50
#define MAX_RANGING_MEASUREMENTS 10

typedef struct
{
  PacketHeader header;
  GroupIdentifier RANGING;
  OpcodeIdentifier RANGE_START;
  uint32_t SequenceNumber;
  uint32_t SessionID;
  uint8_t RCRIndication;
  uint32_t CurrentRangingInterval;
  uint8_t RangingMeasurementType;
  uint8_t MACAddressingModeIndicator;
  uint8_t NumberofRangingMeasurements;
  uint16_t MACAddress[MAX_RANGING_MEASUREMENTS];
  uint8_t Status[MAX_RANGING_MEASUREMENTS];
  uint8_t NLoS[MAX_RANGING_MEASUREMENTS];
  uint16_t Distance[MAX_RANGING_MEASUREMENTS];
} NotificationPacket;

NotificationPacket parse_notif(uint8_t *payload);

#endif // NOTIF_H