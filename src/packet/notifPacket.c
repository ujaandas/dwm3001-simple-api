#include "packet/notifPacket.h"

NotificationPacket parse_notif(uint8_t *payload)
{
  NotificationPacket notif;
  // Assuming payload is at least MIN_PAYLOAD_SIZE bytes
  if (payload == NULL || notif.header.mt != NOTIF)
  {
    return notif;
  }
  printf("ok1");

  notif.SequenceNumber = *((uint32_t *)(payload));
  notif.SessionID = *((uint32_t *)(payload + 4));
  notif.RCRIndication = *(payload + 8);
  notif.CurrentRangingInterval = *((uint32_t *)(payload + 9));
  notif.RangingMeasurementType = *(payload + 13);
  notif.MACAddressingModeIndicator = *(payload + 14);
  notif.NumberofRangingMeasurements = *(payload + 15);

  if (notif.NumberofRangingMeasurements > MAX_RANGING_MEASUREMENTS)
  {
    notif.NumberofRangingMeasurements = MAX_RANGING_MEASUREMENTS;
  }

  for (uint8_t i = 0; i < notif.NumberofRangingMeasurements; ++i)
  {
    uint16_t base_index = 16 + i * 7;
    notif.MACAddress[i] = *((uint16_t *)(payload + base_index));
    notif.Status[i] = *(payload + base_index + 2);
    notif.NLoS[i] = *(payload + base_index + 3);
    notif.Distance[i] = *((uint16_t *)(payload + base_index + 4));
  }

  return notif;
}