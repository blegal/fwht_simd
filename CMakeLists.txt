cmake_minimum_required(VERSION 3.21)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

project(NB_SC_DECODER_EXPLORER LANGUAGES CXX)

add_compile_options (-march=native -mtune=native -O3 -ffast-math)
add_compile_options (-Wall -Wextra)

#######################################################################################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math -g3")

add_executable (nb_sc_decoder   ${CMAKE_CURRENT_SOURCE_DIR}/apps/nb_sc_decoder.cpp
        src/decoders/pruning_fly/decoder_pruned.cpp
        src/decoders/specialized/decoder_specialized.cpp
        src/decoders/classic/decoder_naive.cpp
        src/decoders/prunic_static/decoder_naive_pruning.cpp
)
target_include_directories (nb_sc_decoder PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries (nb_sc_decoder PUBLIC utilities)

#######################################################################################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math -g3")

add_executable(simulator_dec1
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/simulator_dec1.cpp
        src/decoders/pruning_fly/decoder_pruned.cpp
        src/decoders/specialized/decoder_specialized.cpp
        src/decoders/classic/decoder_naive.cpp
        src/decoders/prunic_static/decoder_naive_pruning.cpp
)
target_include_directories (simulator_dec1 PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries      (simulator_dec1 PUBLIC utilities)

#######################################################################################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math -g3")

add_executable(simulator_dec2
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/simulator_dec2.cpp
        src/decoders/pruning_fly/decoder_pruned.cpp
        src/decoders/specialized/decoder_specialized.cpp
        src/decoders/classic/decoder_naive.cpp
        src/decoders/prunic_static/decoder_naive_pruning.cpp
)
target_include_directories (simulator_dec2 PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries      (simulator_dec2 PUBLIC utilities)

#######################################################################################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math -g3")

add_executable(simulator_dec3
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/simulator_dec3.cpp
        src/decoders/pruning_fly/decoder_pruned.cpp
        src/decoders/specialized/decoder_specialized.cpp
        src/decoders/classic/decoder_naive.cpp
        src/decoders/prunic_static/decoder_naive_pruning.cpp
)
target_include_directories (simulator_dec3 PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries      (simulator_dec3 PUBLIC utilities)

#######################################################################################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3 -ffast-math -g3")

add_executable(simulator_dec4
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/simulator_dec4.cpp
        src/decoders/pruning_fly/decoder_pruned.cpp
        src/decoders/specialized/decoder_specialized.cpp
        src/decoders/classic/decoder_naive.cpp
        src/decoders/prunic_static/decoder_naive_pruning.cpp
)
target_include_directories (simulator_dec4 PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries      (simulator_dec4 PUBLIC utilities)

#######################################################################################################################################

add_subdirectory (src)

#######################################################################################################################################

