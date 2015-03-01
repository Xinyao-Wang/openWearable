#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <math.h>

#include "pinconfig.h"
#include "control.h"
#include "motor.h"
#include "spi.h"
#include "tui.h"

#define ANKLE_OS_v 1

int debug;

int main(int argc, char *argv[])
{
  const float frq_hz = 1500;
  static FILE* f_log;

  /* Command line arguments */
  if( (argc == 2) && (strcmp(argv[1],"-v") == 0) ){
    printf("DEBUG MODE.");
    debug = 1;
  }

  printf("\n---------------------\n");
  printf("Welcome to AnkleOS\n");
  printf("---------------------\n");

  /* Controller */
  if(init_control(frq_hz, f_log) != 0){
    perror("Controller initialization failed.\n");
    return -1;
  }
  if(start_control() != 0){
    perror("Controller start failed.\n");
    return -1;
  }

  while(1);
  /* UI */
  if(init_tui() != 0){
    printf("TUI initialization failed.\n");
    return -1;
  }
  if(start_tui() == 1){
    raise(SIGINT);
  }
}

