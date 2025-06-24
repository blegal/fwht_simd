cmake_minimum_required(VERSION 3.21)
#
#
#
#######################################################################################################################################
#
#
#
project(nb_sc_decoder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

# # Do NOT force Release here — let the user choose build type (default Debug is good)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
list(APPEND source_files "${CMAKE_CURRENT_SOURCE_DIR}/apps/nb_sc_decoder.cpp")

add_executable(nb_sc_decoder ${source_files})
#
#
#
#######################################################################################################################################
#
#
#
project(nb_sc_decoder_abd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

# Do NOT force Release here — let the user choose build type (default Debug is good)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
list(APPEND source_files "${CMAKE_CURRENT_SOURCE_DIR}/apps/nb_sc_decoder_abd.cpp")

add_executable(nb_sc_decoder_abd ${source_files})
#
#
#
#######################################################################################################################################
#
#
#
project(bench_fwht LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

# # Do NOT force Release here — let the user choose build type (default Debug is good)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
list(APPEND source_files "${CMAKE_CURRENT_SOURCE_DIR}/apps/bench_fwht.cpp")

add_executable(bench_fwht ${source_files})
# #
#
#
#######################################################################################################################################
#
#
#
