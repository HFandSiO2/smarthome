# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED ENV{IDF_PATH})
    set(ENV{IDF_PATH} "/home/zhf/esp/v6.0.1/esp-idf")
endif()

set(EXTRA_COMPONENT_DIRS
    components/drivers
    components/system
    components/modules
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(smarthome)