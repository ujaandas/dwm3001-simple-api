#ifndef GID_H
#define GID_H

typedef enum
{
  GID_CORE = 0x0,
  GID_SESSION = 0x1,
  GID_RANGING = 0x2,
} GroupIdentifier;

static const char *gid_t_s(GroupIdentifier gid)
{
  switch (gid)
  {
  case GID_CORE:
    return "CORE";
  case GID_SESSION:
    return "SESSION";
  case GID_RANGING:
    return "RANGING";
  default:
    return "UNKNOWN";
  }
}

#endif // GID_H