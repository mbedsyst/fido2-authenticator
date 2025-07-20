# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(FIDO2_Key)

zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app
        ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/transport
        ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/transport
        ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/ui
)

target_sources(app PRIVATE 
        src/app/main.c
        src/app/app_ctx.c
        src/app/fido_main.c
        src/drivers/ui/led_controller.c
        src/drivers/ui/button_controller.c
        src/drivers/transport/usb_transport.c
)