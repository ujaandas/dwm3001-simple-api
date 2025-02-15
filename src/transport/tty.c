#include "transport/tty.h"

int tty_fd = -1;

int tty_init(const char *tty_path, uint32_t baud_rate)
{
  struct termios options;

  // Open the TTY port
  tty_fd = open(tty_path, O_RDWR | O_NOCTTY | O_NDELAY);
  if (tty_fd == -1)
  {
    perror("      tty: Unable to open TTY");
    return -1;
  }

  // Get the current options for the port
  tcgetattr(tty_fd, &options);

  // Set the baud rate
  cfsetispeed(&options, baud_rate);
  cfsetospeed(&options, baud_rate);

  // 8N1: 8 data bits, no parity, 1 stop bit
  options.c_cflag &= ~PARENB; // No parity
  options.c_cflag &= ~CSTOPB; // 1 stop bit
  options.c_cflag &= ~CSIZE;  // Clear the current data size setting
  options.c_cflag |= CS8;     // 8 data bits

  // Enable the receiver and set local mode
  options.c_cflag |= (CLOCAL | CREAD);

  // Apply the settings
  tcsetattr(tty_fd, TCSANOW, &options);

  // Set the port to blocking mode
  fcntl(tty_fd, F_SETFL, 0);

  return 0; // Success
}

int tty_send(const uint8_t *data, size_t len)
{
  if (tty_fd == -1)
  {
    perror("      tty: TTY not initialized");
    return -1;
  }

  ssize_t bytes_written = write(tty_fd, data, len);
  if (bytes_written < 0)
  {
    perror("      tty: Failed to write to TTY");
    return -1;
  }

  printf("      tty: Sent %ld bytes\n", bytes_written);
  return bytes_written; // Return number of bytes sent
}

int tty_rcv(uint8_t *buffer, size_t len)
{
  if (tty_fd == -1)
  {
    perror("      tty: TTY not initialized");
    return -1;
  }

  ssize_t bytes_read = read(tty_fd, buffer, len);
  if (bytes_read < 0)
  {
    perror("      tty: Failed to read from TTY");
    return -1;
  }

  printf("      tty: Received %ld bytes\n", bytes_read);
  return bytes_read; // Return number of bytes received
}

int tty_close()
{
  if (tty_fd != -1)
  {
    close(tty_fd);
    tty_fd = -1;
    return 0; // Success
  }
  return -1; // Failure
}
