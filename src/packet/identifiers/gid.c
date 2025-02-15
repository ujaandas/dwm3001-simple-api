#include "packet/identifiers/gid.h"

const char *gid_t_s(GroupIdentifier gid)
{
  switch (gid)
  {
  case CORE:
    return "CORE";
  case SESSION:
    return "SESSION";
  case RANGING:
    return "RANGING";
  default:
    return "UNKNOWN";
  }
}