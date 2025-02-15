#include "packet/identifiers/gid.h"
#include "packet/identifiers/oid.h"

const char *oid_t_s(GroupIdentifier gid, OpcodeIdentifier oid)
{
  switch (gid)
  {
  case CORE:
    switch (oid)
    {
    case CORE_DEVICE_RESET:
      return "CORE_DEVICE_RESET";
    case CORE_DEVICE_STATUS:
      return "CORE_DEVICE_STATUS";
    case CORE_GET_DEVICE_INFO:
      return "CORE_GET_DEVICE_INFO";
    default:
      return "UNKNOWN";
    }
  case SESSION:
    switch (oid)
    {
    case SESSION_INIT:
      return "SESSION_INIT";
    case SESSION_DEINIT:
      return "SESSION_DEINIT";
    case SESSION_STATUS:
      return "SESSION_STATUS";
    case SESSION_SET_APP_CONFIG:
      return "SESSION_SET_APP_CONFIG";
    case SESSION_GET_APP_CONFIG:
      return "SESSION_GET_APP_CONFIG";
    default:
      return "UNKNOWN";
    }
  case RANGING:
    switch (oid)
    {
    case RANGE_START:
      return "RANGE_START";
    case RANGE_STOP:
      return "RANGE_STOP";
    default:
      return "UNKNOWN";
    }
  default:
    return "UNKNOWN";
  }
}
