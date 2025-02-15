#ifndef MT_H
#define MT_H

typedef enum
{
  COMMAND = 0b001,
  RESPONSE = 0b010,
  NOTIF = 0b011
} MessageType;

const char *mt_t_s(MessageType mt);

#endif // MT_H