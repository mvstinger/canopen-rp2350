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

#include "hardware/timer.h"
#include "drv_timer_alarm.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

static uint32_t timer_us_;
static alarm_id_t alarm_id_;
static volatile bool timer_fired_;

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
    timer_fired_ = true;
    return 0;
}


static void DrvTimerInit(uint32_t freq)
{
    timer_fired_ = false;
    alarm_id_ = 0;
    timer_us_ = freq / 1000000;
}

static void DrvTimerStart(void)
{
    alarm_id_ = add_alarm_in_us(timer_us_, timer_irq_, NULL, true);
}

static uint8_t DrvTimerUpdate(void)
{
    return (timer_fired_) ? 1u : 0u;
}

static uint32_t DrvTimerDelay(void)
{
    return remaining_alarm_time_us(alarm_id_);
}

static void DrvTimerReload(uint32_t reload)
{
    timer_fired_ = false;
    cancel_alarm(alarm_id_);
    alarm_id_ = 0;
    timer_us_ = reload / 1000000;
}

static void DrvTimerStop(void)
{
    cancel_alarm(alarm_id_);
    alarm_id_ = 0;
}
