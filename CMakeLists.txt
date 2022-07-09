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
find_library(CAEN_DGTZ CAENDigitizer REQUIRED)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)
add_subdirectory(src)