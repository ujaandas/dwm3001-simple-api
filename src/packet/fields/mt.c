#include "packet/header/fields/mt.h"

const char *mt_t_s(MessageType mt)
{
  switch (mt)
  {
  case COMMAND:
    return "COMMAND";
  case RESPONSE:
    return "RESPONSE";
  case NOTIF:
    return "NOTIF";
  default:
    return "UNKNOWN";
  }
}