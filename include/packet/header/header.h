#ifndef HEADER_H
#define HEADER_H

#include "fields/mt.h"
#include "fields/pbf.h"

typedef struct
{
  MessageType mt;
  PacketBoundaryFlag pbf;
} PacketHeader;

static const PacketHeader CommandHeader = {COMMAND, COMPLETE};
static const PacketHeader Respheader = {RESPONSE, COMPLETE};
static const PacketHeader NotifHeader = {NOTIF, COMPLETE};

#endif // HEADER_H