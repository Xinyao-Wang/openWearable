/* Copyright 2017-2019 Jonathan Realmuto

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

#include "pruloop.h"
#include "pam.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

// Pam stuff
i2cmux_t* mux;

reservoir_t* reservoir;
pam_t* pam1;
pam_t* pam2;

const uint8_t mux_i2c_add = 0x77;
const uint8_t sensor_i2c_add = 0x28;
const uint8_t res_chan = 0;

const uint8_t pam_pro_ch = 0;
const uint8_t pam_pro_hp_pin = 5;
const uint8_t pam_pro_lp_pin = 4;
const uint8_t pam_sup_ch = 1;
const uint8_t pam_sup_hp_pin = 7;
const uint8_t pam_sup_lp_pin = 6;

// ---------------------------------------------------------------------------
// PRU0
//
// Edit user defined functions below
// ---------------------------------------------------------------------------
void Pru0Init(pru_mem_t* mem) {

}

void Pru0UpdateState(const pru_count_t* c,
                     const param_mem_t* p_,
                     const lut_mem_t* l_,
                     state_t* s_,
                     pru_ctl_t* ctl_) {





}

void Pru0UpdateControl(const pru_count_t* c,
                       const param_mem_t* p_,
                       const lut_mem_t* l_,
                       state_t* s_,
                       pru_ctl_t* ctl_){

}

void Pru0Cleanup(void) {
}

// ---------------------------------------------------------------------------
// PRU1
//
// Edit user defined functions below
// ---------------------------------------------------------------------------
void Pru1Init(pru_mem_t* mem) {

  mux = MuxI2cInit(mux_i2c_add,PCA9544);
//  reservoir = PamReservoirInit(
//                PressureSensorInit(mux,res_chan,sensor_i2c_add));
}

void Pru1UpdateState(const pru_count_t* c,
                     const param_mem_t* p_,
                     const lut_mem_t* l_,
                     state_t* s_,
                     pru_ctl_t* ctl_) {

//  PamReservoirUpdate(reservoir);
//  s_->p_res = PamReservoirGetPressure(reservoir);

}

void Pru1UpdateControl(const pru_count_t* c,
                       const param_mem_t* p_,
                       const lut_mem_t* l_,
                       state_t* s_,
                       pru_ctl_t* ctl_) {

}

void Pru1Cleanup(void) {
}

