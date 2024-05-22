list(APPEND EXTRA_ZEPHYR_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/../zephyr_input_areamap
  )

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr_blink)



# add_library(ui ${SOURCES})


target_sources(app PRIVATE src/main.c)
include_directories(src/)



add_subdirectory(src/user_interface/)
include_directories(src/user_interface/)


add_subdirectory(src/services/sensor/)
include_directories(src/services/sensor/)