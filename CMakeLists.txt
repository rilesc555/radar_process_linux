# CMakeList.txt : CMake project for radar_process_linux, include source and define
# project specific logic here.

#

# Add source to this project's executable.
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

project (radar_threaded)

add_executable (radar_threaded "radar_process_linux.cpp" 
"Utils/utils.h" "Utils/utils.cpp" "Utils/ThreadSafeQueue.h" "Dependencies/KalmanFilter.cpp" "Dependencies/KalmanFilter.h" )

target_include_directories(radar_threaded PRIVATE "./Dependencies/bnet_api/api/include" "./Dependencies/bnet_api/api/bnet_interface/inc" "./Utils" "./Dependencies/eigen-3.4.0/" "./Dependencies")

include("./Dependencies/bnet_api/BNETConfig.cmake")

target_link_libraries(radar_threaded ${BNET_INTERFACE_LIB_DIR}/libbnetinterface.a)




# TODO: Add tests and install targets if needed.
