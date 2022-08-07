cmake_minimum_required(VERSION 3.16) #pch support

set(CMAKE_CXX_STANDARD 17) #c++17

#currently default to debug. Eventually move to release
if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_BUILD_TYPE "Debug")
    message("Building debug")
else()
    message("Building release")
endif()

project(BoxScore)

set(BS_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(BS_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

#find the CAEN pre-reqs
#For Windows, CAENDigitizer is installed non-globally (we assume default pathing cause why would ever be otherwise)
set(CAEN_WIN_LIB_PATH "C:/Program Files/CAEN/Digitizers/Library/lib/x86_64")
set(CAEN_WIN_INCL_PATH "C:/Program Files/CAEN/Digitizers/Library/include")
if(MSVC)
find_library(CAEN_DGTZ CAENDigitizer.lib HINTS ${CAEN_WIN_LIB_PATH} REQUIRED)
else()
find_library(CAEN_DGTZ CAENDigitizer REQUIRED)
endif()

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/yaml-cpp)
add_subdirectory(src)