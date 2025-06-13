cmake_minimum_required(VERSION 3.7)

project(fwht)

SET(CMAKE_BUILD_TYPE Release)

SET (CMAKE_CXX_STANDARD 17)

SET (CMAKE_CXX_FLAGS "")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern")

SET (CMAKE_EXE_LINKER_FLAGS "-lm")

# Generate the source files list
file (GLOB_RECURSE source_files src/*)

# Add includes
include_directories (src)

# Object
add_library (dec-obj OBJECT ${source_files})

add_executable(fwht $<TARGET_OBJECTS:dec-obj>)
