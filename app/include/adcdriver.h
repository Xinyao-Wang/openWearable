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

#ifndef _ADCDRIVER_H_
#define _ADCDRIVER_H_

#include <stdint.h>
#include "fix16.h"

// Vadc/ADC_resolution (1800 mV / 4095 bits)
#define FIX16_BITS2VOLTS 0x7087

// ---- Global ----
extern volatile uint32_t *debug_buff;

// ---- Prototype ----
void adcInit(void);
uint32_t adcSampleChBits(uint8_t ch);
fix16_t adcSampleChmV(uint8_t ch);
void adcCleanup(void);

#endif
