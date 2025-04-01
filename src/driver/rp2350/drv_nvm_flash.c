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

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "stdint.h"
#include "pico/stdlib.h"
#include "pico/flash.h"
#include "hardware/flash.h"
#include "drv_nvm_flash.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/* TODO: place here your timer register definitions */

flash_io_args params;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void     DrvNvmInit  (void);
static uint32_t DrvNvmRead  (uint32_t start, uint8_t *buffer, uint32_t size);
static uint32_t DrvNvmWrite (uint32_t start, uint8_t *buffer, uint32_t size);

void read_flash_cb_(void* /*unused*/);
void write_flash_cb_(void* /*unused*/);

/******************************************************************************
* PUBLIC VARIABLE
******************************************************************************/

/* TODO: rename the variable to match the naming convention:
 *   <device>NvmDriver
 */
const CO_IF_NVM_DRV RP2350FlashNvmDriver = {
    DrvNvmInit,
    DrvNvmRead,
    DrvNvmWrite
};

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

void read_flash_cb_(void* /*unused*/) {
    // Check if intent is to read more than possible to flash
    if ( params.size > FLASH_MAX_SIZE ) { return; }
    // Read flash
    for (uint32_t idx = 0; idx < params.size; idx++) {
        params.buffer[idx] = (const uint8_t)(FLASH_ORIGIN + params.start + idx);
    }
    params.response = params.size;
};

void write_flash_cb_(void* /*unused*/) {
    // Check if intent is to write more than possible to flash
    if ( params.size > FLASH_MAX_SIZE ) { return; }
    // Erase then write flash
    flash_range_erase(FLASH_ORIGIN + params.start, params.size);
    flash_range_program(FLASH_ORIGIN + params.start,
                        params.buffer,
                        params.size);
    params.response = params.size;
};


static void DrvNvmInit(void) {
    // Enable cooperative flash access between cores
    //  NOTE: This must also be called on the other core before accessing flash
    //      or set PICO_FLASH_ASSUME_CORE0_SAFE to 1 in the build flags
    flash_safe_execute_core_init();
}

static uint32_t DrvNvmRead(uint32_t start, uint8_t *buffer, uint32_t size) {
    params.start = start;
    params.buffer = buffer;
    params.size = size;
    params.response = 0u;
    flash_safe_execute(
        write_flash_cb_,
        NULL,
        FLASH_TIMEOUT_MS);
    
    return params.response;
}

static uint32_t DrvNvmWrite(uint32_t start, uint8_t *buffer, uint32_t size) {
    params.start = start;
    params.buffer = buffer;
    params.size = size;
    params.response = 0u;
    flash_safe_execute(
        write_flash_cb_,
        NULL,
        FLASH_TIMEOUT_MS);

    return params.response;
}
