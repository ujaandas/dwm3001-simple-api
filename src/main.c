#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "transport/tty.h"
#include "board/commands.h"

// Define the TTY device path and baud rate
#define TTY_PATH_1 "/dev/tty.usbmodemD02FE9942ABC1"
#define TTY_PATH_2 "/dev/tty.usbmodemCC210983BA011"
#define BAUD_RATE B115200

// Define session vars
#define SESSION_ID 42

int tty_fd_1 = -1;
int tty_fd_2 = -1;

void handle_sigint(int sig)
{
  gtfo(tty_fd_1, SESSION_ID);
  gtfo(tty_fd_2, SESSION_ID);
  exit(getpid());
}

int main()
{
  signal(2, handle_sigint); // 2 is sigint

  // Initialize the TTY port
  printf("Initializing TTY 1...\n");
  if (tty_init(&tty_fd_1, TTY_PATH_1, BAUD_RATE) < 0)
  {
    return -1; // Initialization failed
  }

  printf("Initializing TTY 2...\n");
  if (tty_init(&tty_fd_2, TTY_PATH_2, BAUD_RATE) < 0)
  {
    return -1; // Initialization failed
  }

  // printf("\n -- Getting device info... -- \n");
  // get_device_info(tty_fd_1); // Use this to test, should get 0xDECA0302 as device id
  // get_device_info(tty_fd_2);
  // return 0;

  printf("\n -- Resetting UWB 1... -- \n");
  if (reset_device(tty_fd_1) < 0)
  {
    printf("\n**ERROR: FAILED TO RESET DEVICE**\n");
    gtfo(tty_fd_1, SESSION_ID);
    return -1;
  }
  sleep(3);

  printf("\n -- Resetting UWB 2... -- \n");
  if (reset_device(tty_fd_2) < 0)
  {
    printf("\n**ERROR: FAILED TO RESET DEVICE**\n");
    gtfo(tty_fd_2, SESSION_ID);
    return -1;
  }
  sleep(3);

  printf("\n -- Resetting TTY 1... -- \n");
  if (tty_init(&tty_fd_1, TTY_PATH_1, BAUD_RATE) < 0)
  {
    gtfo(tty_fd_1, SESSION_ID);
    return -2; // Initialization failed
  }

  printf("\n -- Resetting TTY 2... -- \n");
  if (tty_init(&tty_fd_2, TTY_PATH_2, BAUD_RATE) < 0)
  {
    gtfo(tty_fd_2, SESSION_ID);
    return -2; // Initialization failed
  }

  printf("\n -- Initializing UWB 1 session... -- \n");
  if (init_uwb_session(tty_fd_1, SESSION_ID, 0x00) < 0)
  {
    printf("\n**ERROR: FAILED TO INIT UWB SESSION**\n");
    gtfo(tty_fd_1, SESSION_ID);
    return -3;
  }
  sleep(3);

  printf("\n -- Initializing UWB 2 session... -- \n");
  if (init_uwb_session(tty_fd_2, SESSION_ID, 0x00) < 0)
  {
    printf("\n**ERROR: FAILED TO INIT UWB SESSION**\n");
    gtfo(tty_fd_2, SESSION_ID);
    return -3;
  }
  sleep(3);

  printf("\n -- Setting UWB 1 as controller... -- \n");
  if (set_uwb_controller(tty_fd_1, SESSION_ID) < 0)
  {
    printf("\n**ERROR: FAILED TO SET CONTROLLER**\n");
    gtfo(tty_fd_1, SESSION_ID);
    return -4;
  }
  sleep(3);

  printf("\n -- Setting UWB 2 as controlee... -- \n");
  if (set_uwb_controlee(tty_fd_2, SESSION_ID) < 0)
  {
    printf("\n**ERROR: FAILED TO SET PARAMS 1**\n");
    gtfo(tty_fd_2, SESSION_ID);
    return -4;
  }
  sleep(3);

  printf("\n -- Starting UWB 1 ranging... -- \n");
  if (start_uwb_ranging_session(tty_fd_1, SESSION_ID) < 0)
  {
    printf("\n**ERROR: FAILED TO START UWB RANGING**\n");
    gtfo(tty_fd_1, SESSION_ID);
    return -5;
  }
  sleep(3);

  printf("\n -- Starting UWB 2 ranging... -- \n");
  if (start_uwb_ranging_session(tty_fd_2, SESSION_ID) < 0)
  {
    printf("\n**ERROR: FAILED TO START UWB RANGING**\n");
    gtfo(tty_fd_2, SESSION_ID);
    return -5;
  }
  sleep(3);

  printf("\n -- Receiving UWB notifications... -- \n");
  receive_process_notif(tty_fd_2);

  return 1; // Success
}
