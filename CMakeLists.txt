cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)
project(jpeg-decoder)


set(CMAKE_CXX_FLAGS  "-lfftw3 -lm")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIR})

find_package(FFTW REQUIRED)
include_directories(${FFTW_INCLUDES})

#include(../common.cmake)

add_library(decoder-lib SHARED decoder.cpp)
add_library(test-lib SHARED test.cpp)

# add libraries

add_executable(test_baseline
        test_baseline.cpp
        ../contrib/catch_main.cpp)

add_executable(test_progressive
        test_progressive.cpp
        ../contrib/catch_main.cpp)

add_executable(dev_test dev_test.cpp)

# link them

target_include_directories (decoder-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_baseline decoder-lib ${FFTW_LIBRARIES} ${PNG_LIBRARY} ${JPEG_LIBRARIES})
target_link_libraries(test_progressive decoder-lib ${FFTW_LIBRARIES} ${PNG_LIBRARY} ${JPEG_LIBRARIES})

target_link_libraries (dev_test test-lib decoder-lib)
