# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED ENV{IDF_PATH})
    set(ENV{IDF_PATH} "/home/zhf/esp/v6.0.1/esp-idf")
endif()

# 工具链路径（VS Code 内可能未 source export.sh）
set(ENV{PATH} "/home/zhf/.espressif/tools/xtensa-esp-elf/esp-15.2.0_20251204/xtensa-esp-elf/bin:$ENV{PATH}")
set(ENV{PATH} "/home/zhf/.espressif/python_env/idf6.0_py3.14_env/bin:$ENV{PATH}")

set(EXTRA_COMPONENT_DIRS
    components/drivers
    components/system
    components/modules
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(smarthome)