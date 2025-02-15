#ifndef PBF_H
#define PBF_H

typedef enum
{
  COMPLETE = 0b0,
  INCOMPLETE = 0b1
} PacketBoundaryFlag;

const char *pbf_t_s(PacketBoundaryFlag pbf);

#endif // PBF_H