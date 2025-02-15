#ifndef COMMANDS_H
#define COMMANDS_H

#include "transport/tty.h"
#include "packet/controlPacket.h"
#include "packet/notifPacket.h"

extern uint8_t buffer[MAX_PAYLOAD_SIZE + 4];

int reset_device();

int get_device_info();

int init_uwb_session(uint32_t sid, uint8_t stype);

int set_uwb_session_parameters(uint32_t sid, uint8_t stype);

int get_uwb_session_parameters(uint32_t sid);

int start_uwb_ranging_session(uint32_t session_id);

int stop_uwb_ranging_session(uint32_t sid);

int deinit_uwb_session(uint32_t sid);

void receive_process_notif();

void gtfo(uint32_t sid);

#endif // COMMANDS_H