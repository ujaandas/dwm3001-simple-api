#include "packet/header/fields/pbf.h"

const char *pbf_t_s(PacketBoundaryFlag pbf)
{
  switch (pbf)
  {
  case COMPLETE:
    return "COMPLETE";
  case INCOMPLETE:
    return "INCOMPLETE";
  default:
    return "UNKNOWN";
  }
}