cmake_minimum_required(VERSION 3.0)
project(helltexter LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

add_library(hook SHARED hook.cpp)
if (CMAKE_GENERATOR_PLATFORM STREQUAL "Win32")
	set_target_properties(hook PROPERTIES OUTPUT_NAME hook32)
endif()

add_executable(helltexter WIN32 helltexter.cpp)
if (CMAKE_GENERATOR_PLATFORM STREQUAL "Win32")
	set_target_properties(helltexter PROPERTIES OUTPUT_NAME helltexter32)
endif()

if (NOT CMAKE_GENERATOR_PLATFORM STREQUAL "Win32")
	add_executable(uninstall WIN32 uninstall.cpp)
endif()