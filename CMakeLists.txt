# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(FIDO2_Key)

zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/app
        ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers/transport
)

target_sources(app PRIVATE 
        app/main.c
        app/app_ctx.c
        app/fido_main.c
        drivers/ui/led_controller.c
        drivers/ui/button_controller.c
        drivers/transport/usb_transport.c
)