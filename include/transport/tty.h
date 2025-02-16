#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

extern int tty_fd_1;
extern int tty_fd_2;

int tty_init(int *tty_fd, const char *tty_path, uint32_t baud_rate);

int tty_send(int tty_fd, const uint8_t *data, size_t len);

int tty_rcv(int tty_fd, uint8_t *buffer, size_t len);

int tty_close(int tty_fd);

#endif // TTY_H
