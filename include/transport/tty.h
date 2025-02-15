#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

extern int tty_fd;

int tty_init(const char *tty_path, uint32_t baud_rate);

int tty_send(const uint8_t *data, size_t len);

int tty_rcv(uint8_t *buffer, size_t len);

int tty_close();

#endif // TTY_H
