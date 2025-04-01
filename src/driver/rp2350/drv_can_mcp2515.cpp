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

#include "stdio.h"
#include "mcp2515/can.h"
#include "drv_can_mcp2515.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/* TODO: place here your CAN controller register definitions */

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void    DrvCanInit   (void);
static void    DrvCanEnable (uint32_t baudrate);
static int16_t DrvCanSend   (CO_IF_FRM *frm);
static int16_t DrvCanRead   (CO_IF_FRM *frm);
static void    DrvCanReset  (void);
static void    DrvCanClose  (void);

/******************************************************************************
* PUBLIC VARIABLE
******************************************************************************/

/* TODO: rename the variable to match the naming convention:
 *   <device>CanDriver
 */
const CO_IF_CAN_DRV DummyCanDriver = {
    DrvCanInit,
    DrvCanEnable,
    DrvCanRead,
    DrvCanSend,
    DrvCanReset,
    DrvCanClose
};

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void DrvCanInit(void) {
    /* TODO: initialize the CAN controller (don't enable communication) */
    can_ = MCP2515(spi0,
                   20,    // CS pin
                   21,    // TX (MOSI) pin
                   19,    // RX (MISO) pin
                   16);   // SCK pin
    ret = can_.reset();
    if (ret != MCP2515::ERROR_OK) {
        // Repeat error message
        while (true) {
            printf(" cc  CAN bus reset failed with code %i\n", ret);
            sleep_ms(1000);
        };
    }
};

static void DrvCanEnable(uint32_t baudrate) {
    // NOTE: This only accepts the pico-mcp2515 defined rates and fails over to
    //       1Mbps if the rate is not defined.
    CAN_SPEED rate = CAN_1000KBPS;
    switch (baudrate) {
        case 5000:
            rate = CAN_5KBPS;
            break;
        case 10000:
            rate = CAN_10KBPS;
            break;
        case 20000:
            rate = CAN_20KBPS;
            break;
        case 31250:
            rate = CAN_31K25BPS;
            break;
        case 33000:
            rate = CAN_33KBPS;
            break;
        case 40000:
            rate = CAN_40KBPS;
            break;
        case 50000:
            rate = CAN_50KBPS;
            break;
        case 80000:
            rate = CAN_80KBPS;
            break;
        case 83300:
            rate = CAN_83K3BPS;
        case 95000:
            rate = CAN_95KBPS;
            break;
        case 100000:
            rate = CAN_100KBPS;
            break;
        case 125000:
            rate = CAN_125KBPS;
            break;
        case 200000:
            rate = CAN_200KBPS;
            break;
        case 250000:
            rate = CAN_250KBPS;
            break;
        case 500000:
            rate = CAN_500KBPS;
            break;
        case 1000000:
            rate = CAN_1000KBPS;
            break;
        default:
            rate = CAN_1000KBPS;
            break;
    ret = can_.setBitrate(rate, MCP_16MHZ);
    if (ret != MCP2515::ERROR_OK) {
        // Repeat error message
        while (true) {
            printf(" cc  CAN bus setBitrate failed with code %i\n", ret);
            sleep_ms(1000);
        };
    }
    ret = can_.setNormalMode();
    if (ret != MCP2515::ERROR_OK) {
        // Repeat error message
        while (true) {}
            printf(" cc  CAN bus setNormalMode failed with code %i\n", ret);
            sleep_ms(1000);
        };
    }
};

static int16_t DrvCanSend(CO_IF_FRM *frm) {
    struct can_frame outgoing;
    outgoing.can_id = frm->Identifier;
    outgoing.can_dlc = frm->DLC;
    for (uint8_t idx=0; idx<frm->DLC; idx++) {
        outgoing.data[idx] = frm->Data[idx];
    }
    ret = can_.sendMessage(&outgoing);
    if (ret != MCP2515::ERROR_OK) {
        printf(" cc  CAN bus sendMessage failed with code %i\n", ret);
        return (-1);
    }
    return (0u);
};

static int16_t DrvCanRead (CO_IF_FRM *frm) {
    struct can_frame incoming;
    ret = can_.readMessage(&incoming);
    if (ret != MCP2515::ERROR_OK) {
        printf(" cc  CAN bus readMessage failed with code %i\n", ret);
        return (-1);
    }
    return (0u);
};

static void DrvCanReset(void) {
    can_.reset();
};

static void DrvCanClose(void) {
    /* TODO: remove CAN controller from CAN network */
};
