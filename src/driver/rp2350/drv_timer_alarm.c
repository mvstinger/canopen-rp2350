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
#include "hardware/timer.h"
#include "drv_timer_alarm.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

static uint32_t freq_;
static uint32_t duration_us_;
static alarm_id_t alarm_id_;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void     DrvTimerInit   (uint32_t freq);
static void     DrvTimerStart  (void);
static uint8_t  DrvTimerUpdate (void);
static uint32_t DrvTimerDelay  (void);
static void     DrvTimerReload (uint32_t reload);
static void     DrvTimerStop   (void);

/******************************************************************************
* PUBLIC VARIABLE
******************************************************************************/

const CO_IF_TIMER_DRV RP2350AlarmTimerDriver = {
    DrvTimerInit,
    DrvTimerReload,
    DrvTimerDelay,
    DrvTimerStop,
    DrvTimerStart,
    DrvTimerUpdate
};

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

int64_t timer_irq_(alarm_id_t /*id*/, void* /*user_data*/) {
    DrvTimerUpdate();
    return 0; // 0: Don't automatically restart the timer
}


// NOTE: Using the "delta" method outlined here with the pico SDK's alarms:
// https://canopen-stack.org/v4.4/hardware/timer/

static void DrvTimerInit(uint32_t freq)
{
    // Alarms use us resolution; Cannot honor alarms faster than 1 MHz
    freq_ = (freq > 1000000) ? 1000000 : freq;
}

static void DrvTimerStart(void)
{
    uint32_t duration = duration_us_;
    while (true) {
        alarm_id_ = add_alarm_in_us(duration_us_,
                                    timer_irq_,
                                    NULL,
                                    false);
        if (alarm_id_ <= 0) {
            printf("[ CAN    ] ****** Timer: Failed to start timer %u\n",
                alarm_id_);
            duration *= 2;
            printf("[ CAN    ] ******   - Retrying with initial duration %u us\n",
                duration);
            return;
        } else {
            break;
        }
    }
}

static uint8_t DrvTimerUpdate(void)
{
    // Delta mode- always returns 1u
    return 1u;
}

static uint32_t DrvTimerDelay(void)
{
    // Duration [ticks] = frequency [ticks/s] * remaining time [us] * (1 s / 1e6 us)
    return remaining_alarm_time_us(alarm_id_) * freq_ / 1000000u;
}

static void DrvTimerReload(uint32_t reload)
{
    // Duration [us] = # ticks / frequency [ticks/s] * (1e6 us / 1 s)
    duration_us_ = reload / freq_ * 1000000u;
}

static void DrvTimerStop(void)
{
    if (!cancel_alarm(alarm_id_)) {
        printf("[ CAN    ] ****** Timer: Failed to cancel alarm %u\n",
            alarm_id_);
        return;
    };
    alarm_id_ = 0;
}
