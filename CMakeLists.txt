# CMakeList.txt : CMake project for radar_process_linux, include source and define
# project specific logic here.

#

# Add source to this project's executable.
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

project (radar_process)

add_executable (radar_process radar_process_linux.cpp ./Utils/utils.cpp	./Dependencies/KalmanFilter.cpp	./Utils/utils.h	./Utils/ThreadSafeQueue.h ./Dependencies/KalmanFilter.h Utils/parsers.h Utils/parsers.cpp)

target_include_directories(radar_process PRIVATE "Dependencies/bnet_api/API/include" "Dependencies/bnet_api/API/bnet_interface/inc" "Utils" "Dependencies/eigen-3.4.0/" "Dependencies")

include(./Dependencies/bnet_api/BNETConfig.cmake)

target_link_libraries(radar_process ${BNET_INTERFACE_LIB_DIR}/libbnetinterface.a pthread)





# TODO: Add tests and install targets if needed.
