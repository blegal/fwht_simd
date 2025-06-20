cmake_minimum_required(VERSION 3.7)

project(fwht)

SET(CMAKE_BUILD_TYPE Release)

SET (CMAKE_CXX_STANDARD 17)

SET (CMAKE_CXX_FLAGS "")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native")

SET (CMAKE_EXE_LINKER_FLAGS "-lm")

# Generate the source files list
project(nb_sc_decoder)

file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

list(APPEND source_files "${CMAKE_CURRENT_SOURCE_DIR}/apps/bench_fwht.cpp")

add_executable(fwht ${source_files})

#
#
########################################################################################################################
#
#
project(nb_sc_decoder)

file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

list(APPEND source_files "${CMAKE_CURRENT_SOURCE_DIR}/apps/nb_sc_decoder.cpp")

add_executable(nb_sc_decoder ${source_files})
