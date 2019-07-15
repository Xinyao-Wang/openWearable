/* Copyright 2017 Jonathan Realmuto

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
=============================================================================*/

#include "ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "log.h"
#include "pru.h"
#include "roshelper.h"

// Global
volatile sig_atomic_t input_ready;
ui_data_t uidata;

// ---------------------------------------------------------------------------
static void UiInputCallback(int sig) {
  input_ready = 1;
}

static void UiTimerCallback(int sig) {

  // always update circular buffer
  LogCircBuffUpdate(uidata.log);

  if (uidata.flag.logging)
    LogWriteStateToFile(uidata.log);
  if (uidata.flag.udppublish)
    UdpPublish(uidata.log, uidata.udp);
}
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Function init_tui()
// ----------------------------------------------------------------------------
ui_flags_t UiInitFlags(void) {
  ui_flags_t f;
  f.debug = 0;
  f.logging = 0;
  f.logfile = 0;
  f.rospublish = 0;
  f.udppublish = 0;
  return f;
}

int UiInit(pru_mem_t* pru_mem, ui_flags_t flags) {

  uidata.flag = flags;
  uidata.log = LogInit(pru_mem);

  if (uidata.flag.udppublish)
    uidata.udp = UdpInit();

  if (uidata.flag.rospublish)
    uidata.ros = RosPubInit();

  // Setup stdin flags
  if (fcntl(0, F_SETOWN, getpid()) == -1) {
    printf("F_SETOWN error.\n");
    return -1;
  }
  if (fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_ASYNC | O_NONBLOCK) == -1) {
    printf("Error setting stdin fd flags.\n");
    return -1;
  }

  // Setup action for SIGIO for user inputs
  struct sigaction action_ui;
  action_ui.sa_handler = UiInputCallback;
  sigemptyset(&action_ui.sa_mask);
  action_ui.sa_flags = 0;
  if (sigaction(SIGIO, &action_ui, NULL) == -1)
    printf("Error sigaction\n");

  // Setup action for SIGVTALRM
  struct sigaction action_timer;
  action_timer.sa_handler = UiTimerCallback;
  sigemptyset(&action_timer.sa_mask);
  action_timer.sa_flags = 0;
  if (sigaction(SIGVTALRM, &action_timer, NULL) == -1)
    printf("Error sigvtalrm\n");

  // Configure timer
	struct itimerval timer;
 	timer.it_value.tv_sec = 0;
 	timer.it_value.tv_usec = 33000;
 	timer.it_interval.tv_sec = 0;
 	timer.it_interval.tv_usec = 33000;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);

  printf("TUI initialized.\n");
  fflush(stdout);
  return 0;
}

//int UiInitLogAndPublishThread(const pru_mem_t* pru_mem) {
//
//  thread_data.logflag = 0;
//  thread_data.udp = UdpInit();
//  thread_data.log = LogInit(pru_mem);
//
//  if (PruOwModeRos(pru_mem))
//    thread_data.rp = RosPubInit();
//
//  if (pthread_mutex_init(&lock, NULL) != 0) {
//    printf("\n mutex init failed\n");
//    return 1;
//  }
//  if (pthread_create(&thread, NULL, &UiLogAndPublishThread, NULL)) {
//    printf("\n thread create failed\n");
//    return 1;
//  }

//  return 0;
//}


void UiStartLog(void) {
  uidata.flag.logging = 1;
}

void UiStopAndSaveLog(void) {
  if (uidata.flag.logging)
    LogSaveFile(uidata.log);

  uidata.flag.logging = 0;
  uidata.flag.logfile = 0;
}

void UiNewLogFile(char* log_file) {
  uidata.flag.logfile = 1;
  LogNewFile(uidata.log, log_file);
}


void UiPollForUserInput(void) {
  input_ready = 0;
  while (1)
    if (input_ready)
      break;
}

void UiWelcome(void) {
  printf("\n"
         "-----------------------------\n"
         " Welcome to openWearable v0.1\n"
         "-----------------------------\n");
  printf("\n\nPress enter to start\n\n");
  getchar();
}

void UiSetPruCtlBit(const pru_mem_t* pru_mem, unsigned char n) {
  pru_mem->s->pru_ctl.bit.utility |= (1 << n);
}

void UiClearPruCtlBit(const pru_mem_t* pru_mem, unsigned char n) {
  pru_mem->s->pru_ctl.bit.utility &= ~(1 << n);
}

void UiPollPruCtlBit(const pru_mem_t* pru_mem, unsigned char n) {
  while (1)
    if ((pru_mem->s->pru_ctl.bit.utility & (1 << n)) == (1 << n))
      break;
}

int UiCleanup(void) {
  if(fcntl(0, F_SETOWN, NULL) == -1){
    printf("F_SETOWN error.\n");
    return -1;
  }
  if(fcntl(0, F_SETFL, fcntl(0, F_GETFL) & ~O_ASYNC & ~O_NONBLOCK) == -1){
    printf("Error setting stdin fd flags.\n");
    return -1;
  }
  LogCleanup(uidata.log);
  if (uidata.flag.rospublish)
    RosPubCleanup(uidata.ros);
  printf("TUI cleaned up.\n");
  return 0;
}

