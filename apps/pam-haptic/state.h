#ifndef _STATE_
#define _STATE_

#include "fix16.h"
#include "pam.h"


// State
//
//  packing can be issue -- use 32bit types
//
typedef struct {
  volatile uint32_t time;
  volatile fix16_t p_res;
  volatile pam_state_t pam1_state;
  volatile pam_state_t pam2_state;
  volatile fix16_t x;
  volatile fix16_t xd;
 } state_t;

// Parameters
typedef struct {
  volatile uint32_t debug_buff[10];
  uint32_t fs_hz;
  uint32_t fs_ticks;
  uint32_t Td;  // cycle period (in clock cnts)
  uint32_t Np;  // number of cycles
  fix16_t pd;  // resting pressure
} param_mem_t;


#endif
