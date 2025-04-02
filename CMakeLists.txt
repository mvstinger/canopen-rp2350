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

cmake_policy(SET CMP0057 NEW)


# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in SDK (must be before project)
#   Fetch SDK from github if not locally present
set(PICO_SDK_PATH /home/mvstinger/sdk/pico/pico-sdk)
if (NOT DEFINED PICO_SDK_PATH)
    message(STATUS "Pico SDK not found (PICO_SDK_PATH not set), fetching from git")
    set(PICO_SDK_FETCH_FROM_GIT ON)
else()
    message(STATUS "Using Pico SDK found at ${PICO_SDK_PATH}")
endif()
#   Set board type
set(PICO_PLATFORM "rp2350")
set(PICO_BOARD "pico2")
include(pico_sdk_import.cmake)


cmake_minimum_required(VERSION 3.20)     # buildPresets is introduced in 3.20

project(canopen-rp2350)

enable_language(C CXX ASM)
enable_testing()


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Initialize the Pico SDK
pico_sdk_init()


# Secure dependencies
# include(FetchContent)
include(cmake/CPM.cmake)
#   pico-MCP2515
message(STATUS "Fetching pico-mcp2515")
FetchContent_Declare(
    pico-mcp2515
    GIT_REPOSITORY  https://github.com/adamczykpiotr/pico-mcp2515.git
    GIT_TAG         4506dd0659b37b35eaf411de2d93669da471f6c4)
#  canopen-stack
set(CO_TARGET   "canopen-stack")
set(CO_PROJECT  "embedded-office/canopen-stack")
set(CO_VERSION  "4.4.0")
CPMAddPackage(
  NAME    canopen-stack
  URL     https://github.com/${CO_PROJECT}/releases/download/v${CO_VERSION}/${CO_TARGET}-src.zip
  VERSION ${CO_VERSION}
)
#   Wait for fetched content
FetchContent_MakeAvailable(pico-mcp2515)


# Target definitions
add_subdirectory(src)
# add_subdirectory(tests)
