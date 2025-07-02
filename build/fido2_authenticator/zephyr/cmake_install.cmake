# Install script for directory: /home/mbedsyst/ncs/v3.0.0/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/mbedsyst/ncs/toolchains/7cbc0036f4/opt/zephyr-sdk/arm-zephyr-eabi/bin/arm-zephyr-eabi-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/zephyr/arch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/zephyr/lib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/zephyr/soc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/zephyr/boards/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/zephyr/subsys/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/zephyr/drivers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/nrf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/hostap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/mcuboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/trusted-firmware-m/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/cjson/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/azure-sdk-for-c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/cirrus-logic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/openthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/suit-processor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/memfault-firmware-sdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/canopennode/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/chre/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/lz4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/nanopb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/zscilib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/cmsis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/cmsis-dsp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/cmsis-nn/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/fatfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/hal_nordic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/hal_st/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/hal_tdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/hal_wurthelektronik/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/liblc3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/libmetal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/littlefs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/loramac-node/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/lvgl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/mipi-sys-t/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/nrf_wifi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/open-amp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/percepio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/picolibc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/segger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/tinycrypt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/uoscore-uedhoc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/zcbor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/nrfxlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/nrf_hw_models/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/modules/connectedhomeip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/zephyr/kernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/zephyr/cmake/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/zephyr/cmake/usage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/mbedsyst/Lab/Nordic/ncs/fido2_authenticator/build/fido2_authenticator/zephyr/cmake/reports/cmake_install.cmake")
endif()

