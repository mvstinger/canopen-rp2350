#******************************************************************************
#   Copyright (c) 2025 Michael Stinger
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#******************************************************************************

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
    driver/rp2350/drv_can_mcp2515.cpp
    driver/rp2350/drv_nvm_flash.c
    driver/rp2350/drv_timer_alarm.c
    ${pico-mcp2515_SOURCE_DIR}/include/mcp2515/mcp2515.cpp)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    driver/rp2350
    ${PICO_SDK_DIR}/include
    ${pico-mcp2515_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    pico_stdlib
    pico_flash
    hardware_spi
    canopen-stack)


# Enable USB printf output, disable UART printf output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)