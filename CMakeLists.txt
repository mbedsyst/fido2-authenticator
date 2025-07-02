# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(fido2_authenticator)

target_include_directories(app PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/logger
)

target_sources(app PRIVATE 
        src/main.c
        utils/logger/logger.c
)
