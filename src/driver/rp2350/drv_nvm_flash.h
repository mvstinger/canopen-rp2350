/******************************************************************************
   Copyright 2020 Embedded Office GmbH & Co. KG

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************************/

#ifndef CO_NVM_FLASH_H_
#define CO_NVM_FLASH_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "stdint.h"
#include "pico/stdlib.h"
#include "co_if.h"

/******************************************************************************
* PUBLIC SYMBOLS
******************************************************************************/

extern const CO_IF_NVM_DRV RP2350FlashNvmDriver;

typedef struct {
    uint32_t start;
    uint8_t *buffer;
    uint32_t size;
    uint32_t response;
} flash_io_args;

#define FLASH_TIMEOUT_MS (unsigned long int)1000
// Set a flash I/O origin at 3 MB after the start of flash
//  (Must be multiple of 4 kB)
//  Raspberry Pi Pico2 has 4 MB of flash
#define FLASH_ORIGIN (XIP_BASE + (3*1024*1024))
#define FLASH_MAX_SIZE ((4*1024*1024) - FLASH_ORIGIN)

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
