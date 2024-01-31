# CMakeList.txt : CMake project for radar_process_linux, include source and define
# project specific logic here.
#

# Add source to this project's executable.
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

project (radar_process)

add_executable (radar_process "radar_process_linux.cpp" "radar_process_linux.h")


include_directories("./bnet_api/api/include" "./bnet_api/api/bnet_interface/inc")

include("./bnet_api/BNETConfig.cmake")

target_link_libraries(radar_process ${BNET_INTERFACE_LIB_DIR}/libbnetinterface.a)




# TODO: Add tests and install targets if needed.
