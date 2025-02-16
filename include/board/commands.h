#ifndef COMMANDS_H
#define COMMANDS_H

#include "transport/tty.h"
#include "packet/controlPacket.h"
#include "packet/notifPacket.h"

extern uint8_t buffer[MAX_PAYLOAD_SIZE + 4];

int reset_device(int tty_fd);

int get_device_info(int tty_fd);

int init_uwb_session(int tty_fd, uint32_t sid, uint8_t stype);

int set_uwb_controller(int tty_fd, uint32_t sid);

int set_uwb_controlee(int tty_fd, uint32_t sid);

int set_uwb_misc_1(int tty_fd, uint32_t sid);

int set_uwb_misc_2(int tty_fd, uint32_t sid);

int set_uwb_session_parameters(int tty_fd, uint32_t sid, uint8_t session_params[], uint8_t session_params_len);

int get_uwb_session_parameters(int tty_fd, uint32_t sid);

int start_uwb_ranging_session(int tty_fd, uint32_t session_id);

int stop_uwb_ranging_session(int tty_fd, uint32_t sid);

int deinit_uwb_session(int tty_fd, uint32_t sid);

void receive_process_notif(int tty_fd);

void gtfo(int tty_fd, uint32_t sid);

#endif // COMMANDS_H