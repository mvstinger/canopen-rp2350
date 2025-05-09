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

#ifndef CO_CAN_MCP2515_H_
#define CO_CAN_MCP2515_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "mcp2515/mcp2515.h"
#include "co_if.h"

/******************************************************************************
* PUBLIC SYMBOLS
******************************************************************************/

extern const CO_IF_CAN_DRV RP2350MCP2515CanDriver;

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
