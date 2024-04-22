# BNETConfig.cmake
#
# Configures the BNET library.
#
# This will define the following variables
#
#       BNET_FOUND
#       BNET_INCLUDE_DIRS
#
# And the following imported targets (static lib default)
#
#       BNET::bnet_interface
#       BNET::bnet_reader
#       BNET::bnet_plus
#
#       BNET::bnet_interface_dynamic
#       BNET::bnet_plus_dynamic
#
# Author: Bryan Womack
# Date: February 18, 2020

message("<BNETConfig.cmake>")

# Properly choose the path to search in BNET release folder
IF (WIN32)
    SET(BNET_OS_DIR Windows)
    SET(SHARED_LIB_EXT dll)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        SET(STATIC_LIB_EXT lib)
		SET(STATIC_LIB_PREFIX "")
    else()
        SET(STATIC_LIB_EXT a)
		SET(STATIC_LIB_PREFIX lib)
    endif()
ELSE()
    SET(BNET_OS_DIR Linux)
    SET(SHARED_LIB_EXT so)
    SET(STATIC_LIB_EXT a)
	SET(STATIC_LIB_PREFIX lib)
ENDIF()

SET(BNET_SEARCH_PATHS
    ${BNET_DIR}/${BNET_OS_DIR}
    ${BNET_DIR}
)

# Find the location of BNET API in release
FIND_PATH(BNET_API_DIR bnet_interface
    HINTS
        $ENV{BNET_DIR}
    PATH_SUFFIXES
        API
    PATHS
        ${BNET_SEARCH_PATHS}
)

# Find the location of BNET API Common Includes
FIND_PATH(BNET_API_INCLUDE_DIR bnet_data.h
    HINTS
        $ENV{BNET_DIR}
    PATH_SUFFIXES
        API/include
        include
    PATHS
        ${BNET_SEARCH_PATHS}
)

# Create Target BNET::includes
add_library(BNET::includes INTERFACE IMPORTED)
set_target_properties(BNET::includes PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES
        ${BNET_API_INCLUDE_DIR}
)

# Create Target BNET::bnet_interface
SET(BNET_INTERFACE_INCLUDE_DIR ${BNET_API_DIR}/bnet_interface/inc)
SET(BNET_INTERFACE_LIB_DIR ${BNET_API_DIR}/bnet_interface/lib)
SET(BNET_INTERFACE_BIN_DIR ${BNET_API_DIR}/bnet_interface/bin)
add_library(BNET::bnet_interface STATIC IMPORTED)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
set_target_properties(BNET::bnet_interface PROPERTIES
    IMPORTED_LOCATION
	${BNET_INTERFACE_LIB_DIR}/${STATIC_LIB_PREFIX}bnetinterface.${STATIC_LIB_EXT}
    INTERFACE_LINK_LIBRARIES
        "BNET::includes;ws2_32"
    INTERFACE_INCLUDE_DIRECTORIES
        ${BNET_INTERFACE_INCLUDE_DIR}
    CXX_STANDARD
        17
    CXX_STANDARD_REQUIRED
        YES
    COMPILE_FEATURES
        cxx_std_17
)
else()
set_target_properties(BNET::bnet_interface PROPERTIES
    IMPORTED_LOCATION
	${BNET_INTERFACE_LIB_DIR}/${STATIC_LIB_PREFIX}bnetinterface.${STATIC_LIB_EXT}
    INTERFACE_LINK_LIBRARIES
        "BNET::includes;pthread"
    INTERFACE_INCLUDE_DIRECTORIES
        ${BNET_INTERFACE_INCLUDE_DIR}
    CXX_STANDARD
        17
    CXX_STANDARD_REQUIRED
        YES
    COMPILE_FEATURES
        cxx_std_17
)
endif()

IF (WIN32)
    add_library(BNET::bnet_interface_dynamic_static STATIC IMPORTED)
    add_library(BNET::bnet_interface_dynamic SHARED IMPORTED)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set_target_properties(BNET::bnet_interface_dynamic_static PROPERTIES
            IMPORTED_LOCATION
            ${BNET_INTERFACE_LIB_DIR}/${STATIC_LIB_PREFIX}bnetinterface.${STATIC_LIB_EXT}
        )
    else()
        set_target_properties(BNET::bnet_interface_dynamic_static PROPERTIES
            IMPORTED_LOCATION
            ${BNET_INTERFACE_LIB_DIR}/${STATIC_LIB_PREFIX}bnetinterface.${SHARED_LIB_EXT}.${STATIC_LIB_EXT}
        )
    endif()
    set_target_properties(BNET::bnet_interface_dynamic PROPERTIES
        IMPORTED_LOCATION
            ${BNET_INTERFACE_BIN_DIR}/${STATIC_LIB_PREFIX}bnetinterface.${SHARED_LIB_EXT}
        INTERFACE_LINK_LIBRARIES
            "BNET::includes;BNET::bnet_interface_dynamic_static"
    )
ELSE()
    add_library(BNET::bnet_interface_dynamic SHARED IMPORTED)
    set_target_properties(BNET::bnet_interface_dynamic PROPERTIES
        IMPORTED_LOCATION
            ${BNET_INTERFACE_LIB_DIR}/${STATIC_LIB_PREFIX}bnetinterface.${SHARED_LIB_EXT}
        INTERFACE_LINK_LIBRARIES
            "BNET::includes"
    )
ENDIF()
set_target_properties(BNET::bnet_interface_dynamic PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES
        ${BNET_INTERFACE_INCLUDE_DIR}
    CXX_STANDARD
        17
    CXX_STANDARD_REQUIRED
        YES
    COMPILE_FEATURES
        cxx_std_17
)

# Create Target BNET::bnet_reader
SET(BNET_READER_INCLUDE_DIR ${BNET_API_DIR}/bnet_reader/inc)
SET(BNET_READER_LIB_DIR ${BNET_API_DIR}/bnet_reader/lib)
add_library(BNET::bnet_reader STATIC IMPORTED)
set_target_properties(BNET::bnet_reader PROPERTIES
    IMPORTED_LOCATION
        ${BNET_READER_LIB_DIR}/${STATIC_LIB_PREFIX}bnetreader.a
    INTERFACE_LINK_LIBRARIES
        "BNET::includes"
    INTERFACE_INCLUDE_DIRECTORIES
        ${BNET_READER_INCLUDE_DIR}
    CXX_STANDARD
        17
    CXX_STANDARD_REQUIRED
        YES
    COMPILE_FEATURES
        cxx_std_17
)

# Create Target BNET::bnet_plus
SET(BNET_PLUS_INCLUDE_DIR ${BNET_API_DIR}/bnet_plus/inc)
SET(BNET_PLUS_LIB_DIR ${BNET_API_DIR}/bnet_plus/lib)
SET(BNET_PLUS_BIN_DIR ${BNET_API_DIR}/bnet_plus/bin)
add_library(BNET::bnet_plus STATIC IMPORTED)
set_target_properties(BNET::bnet_plus PROPERTIES
    IMPORTED_LOCATION
        ${BNET_PLUS_LIB_DIR}/${STATIC_LIB_PREFIX}bnetplus.a
    INTERFACE_LINK_LIBRARIES
        "BNET::includes;BNET::bnet_interface;BNET::bnet_reader"
    INTERFACE_INCLUDE_DIRECTORIES
        ${BNET_PLUS_INCLUDE_DIR}
    CXX_STANDARD
        17
    CXX_STANDARD_REQUIRED
        YES
    COMPILE_FEATURES
        cxx_std_17
)
if (WIN32)
    add_library(BNET::bnet_plus_dynamic_static STATIC IMPORTED)
    set_target_properties(BNET::bnet_plus_dynamic_static PROPERTIES
        IMPORTED_LOCATION
            ${BNET_PLUS_LIB_DIR}/${STATIC_LIB_PREFIX}bnetplus.${SHARED_LIB_EXT}.a
    )
    add_library(BNET::bnet_plus_dynamic SHARED IMPORTED)
    set_target_properties(BNET::bnet_plus_dynamic PROPERTIES
        IMPORTED_LOCATION
            ${BNET_PLUS_BIN_DIR}/${STATIC_LIB_PREFIX}bnetplus.${SHARED_LIB_EXT}
        INTERFACE_LINK_LIBRARIES
            "BNET::includes;BNET::bnet_interface_dynamic;BNET::bnet_reader;BNET::bnet_plus_dynamic_static"
    )
ELSE()
    add_library(BNET::bnet_plus_dynamic SHARED IMPORTED)
    set_target_properties(BNET::bnet_plus_dynamic PROPERTIES
        IMPORTED_LOCATION
            ${BNET_PLUS_LIB_DIR}/${STATIC_LIB_PREFIX}bnetplus.${SHARED_LIB_EXT}
        INTERFACE_LINK_LIBRARIES
            "BNET::includes;BNET::bnet_interface_dynamic;BNET::bnet_reader"
    )
ENDIF()
set_target_properties(BNET::bnet_plus_dynamic PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES
        ${BNET_PLUS_INCLUDE_DIR}
    CXX_STANDARD
        17
    CXX_STANDARD_REQUIRED
        YES
    COMPILE_FEATURES
        cxx_std_17
)
