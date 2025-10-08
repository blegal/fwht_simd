cmake_minimum_required(VERSION 3.21)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

project(NB_SC_DECODER_EXPLORER LANGUAGES CXX)

add_compile_options (-march=native -mtune=native -O3 -g0 -ffast-math) #
# add_compile_options(-g -O0 -march=native -mtune=native -Wall -Wextra)#debug2
add_compile_options (-Wall -Wextra -Werror)

#######################################################################################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math -g3")

#add_executable (nb_sc_decoder   ${CMAKE_CURRENT_SOURCE_DIR}/apps/nb_sc_decoder.cpp
#        src/decoders/specialized_pruning/decoder_specialized_pruning.cpp
#        src/decoders/specialized/decoder_specialized.cpp
#        src/decoders/naive/decoder_naive.cpp
#        src/decoders/naive_pruning/decoder_naive_pruning.cpp
#)
#target_include_directories (nb_sc_decoder PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
#target_link_libraries (nb_sc_decoder PUBLIC utilities)

#######################################################################################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math -g3")

add_executable(benchmarking
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/benchmarking.cpp
        src/decoders/specialized_pruning/decoder_specialized_pruning.cpp
        src/decoders/specialized/decoder_specialized.cpp
        src/decoders/dedicated/decoder_dedicated.cpp
        src/decoders/nb_sc_decoder/nb_sc_decoder.cpp
        src/decoders/naive/decoder_naive.cpp
        src/decoders/naive_pruning/decoder_naive_pruning.cpp
        src/encoder/polar_encoder.cpp
)
target_include_directories (benchmarking PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries      (benchmarking PUBLIC utilities)

#######################################################################################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math -g3")

add_executable(fwht_validator
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/bench_fwht.cpp
)
target_include_directories (fwht_validator PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries      (fwht_validator PUBLIC utilities)

#######################################################################################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math -g3")

add_executable(code_generator
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/code_generator.cpp
        src/encoder/polar_encoder.cpp
)
target_include_directories (code_generator PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries      (code_generator PUBLIC utilities)

#######################################################################################################################################

add_subdirectory (src)

#######################################################################################################################################

add_library(NbScFFTdec SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/apps/LibNbScFFTdec.cpp
    src/encoder/polar_encoder.cpp
)
target_include_directories (NbScFFTdec PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries      (NbScFFTdec PUBLIC utilities)

#######################################################################################################################################
