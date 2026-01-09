cmake_minimum_required (VERSION 3.21)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_BUILD_TYPE Release)

set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)

project (NB_SC_DECODER_EXPLORER LANGUAGES CXX)

option (SANITIZE_DECODERS "Add sanitization values" OFF)

set (DEC_NO 0 CACHE STRING "Decoder type to build")
set (Ns "8;16;32;32;64;64;64;64;64;64;64;64;64;64;128;128;128;256;256;256;256;512;1024;1024" CACHE INTERNAL
                                                                                                   "N values to build a decoder for"
)
set (GFs "64;64;64;256;8;16;32;64;128;256;512;1024;2048;4096;64;128;256;16;64;128;256;64;64;256" CACHE INTERNAL
                                                                                                       "GF values to build a decoder for"
)
set (Rs "0.25;0.5;0.75;0.80;0.90" CACHE INTERNAL "Rate values to build a decoder 5 for")

if (SANITIZE_DECODERS)
  set (SANITIZE_CXXFLAGS "-fsanitize=address;-fsanitize=undefined;-fno-omit-frame-pointer;-ggdb")
  set (SANITIZE_LDFLAGS "-fsanitize=address;-fsanitize=undefined;-fno-omit-frame-pointer;-ggdb")
endif ()

# SET (CMAKE_C_COMPILER   /opt/homebrew/opt/llvm/bin/clang)     # LLVM 21.1.7 SET (CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++) #
# LLVM 21.1.7

# SET (CMAKE_C_COMPILER   /opt/homebrew/Cellar/llvm@20/20.1.8/bin/clang)     # LLVM 20.1.8 SET (CMAKE_CXX_COMPILER
# /opt/homebrew/Cellar/llvm@20/20.1.8/bin/clang++)   # LLVM 20.1.8

# SET (CMAKE_C_COMPILER   /opt/homebrew/Cellar/llvm@18/18.1.8/bin/clang)     # LLVM 18.1.8 SET (CMAKE_CXX_COMPILER
# /opt/homebrew/Cellar/llvm@18/18.1.8/bin/clang++)   # LLVM 18.1.8

# SET (CMAKE_C_COMPILER   /opt/homebrew/bin/gcc-13)     # GCC 13.4.0 SET (CMAKE_CXX_COMPILER /opt/homebrew/bin/g++-13)   # GCC 13.4.0

# SET (CMAKE_C_COMPILER   /opt/homebrew/bin/gcc-15)     # GCC 15.2.0 SET (CMAKE_CXX_COMPILER /opt/homebrew/bin/g++-15)   # GCC 15.2.0

add_compile_options (
  -march=native
  -mtune=native
  -O3
  -g0
  ${SANITIZE_CXXFLAGS}
)
# add_compile_options(-g -O0 -march=native -mtune=native -Wall -Wextra ${SANITIZE_CXXFLAGS})#debug2
add_compile_options (-Wall -Wextra)
# to remove warnings from ap_int
add_compile_options (
  -Wno-unused-parameter
  -Wno-deprecated-copy
  -Wno-int-in-bool-context
  -Wno-type-limits
  -Wno-strict-aliasing
  -Wno-extra
)
add_link_options (${SANITIZE_LDFLAGS})

# ##########################################################################################################################################

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3
# -ffast-math -g3")

# add_executable (nb_sc_decoder   ${CMAKE_CURRENT_SOURCE_DIR}/apps/nb_sc_decoder.cpp
# src/decoders/specialized_pruning/decoder_specialized_pruning.cpp src/decoders/specialized/decoder_specialized.cpp
# src/decoders/naive/decoder_naive.cpp src/decoders/naive_pruning/decoder_naive_pruning.cpp ) target_include_directories (nb_sc_decoder
# PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common) target_link_libraries (nb_sc_decoder PUBLIC utilities)

# ##########################################################################################################################################

find_program (PYTHON_EXECUTABLE NAMES python3 python)
if (NOT PYTHON_EXECUTABLE)
  message (FATAL_ERROR "Python not found! Cannot perform floating-point calculations.")
endif ()

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3
# -ffast-math -g3")

add_executable (
  benchmarking EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/apps/benchmarking.cpp
  src/decoders/specialized_pruning/decoder_specialized_pruning.cpp
  src/decoders/specialized/decoder_specialized.cpp
  src/decoders/dedicated/decoder_dedicated.cpp
  src/decoders/naive/decoder_naive.cpp
  src/decoders/basic/decoder_basic.cpp
  src/decoders/naive_fixed/decoder_naive_fixed.cpp
  src/decoders/naive_int32_t/decoder_naive_int32_t.cpp
  src/decoders/naive_cfloat/decoder_naive_cfloat.cpp
  src/hadamard/Hadamard.cpp
  src/encoder/polar_encoder.cpp
)
target_include_directories (benchmarking PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries (benchmarking PUBLIC utilities)

foreach (N GF IN ZIP_LISTS Ns GFs)
  set (SRC_DIR_N_GF ${CMAKE_BINARY_DIR}/src_N${N}_GF${GF})
  file (MAKE_DIRECTORY ${SRC_DIR_N_GF}/definitions)

  if (DEC_NO EQUAL 5)
    file (MAKE_DIRECTORY ${SRC_DIR_N_GF}/generator)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/src/definitions/code.hpp.in ${SRC_DIR_N_GF}/definitions/code.hpp)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/src/generator/dec_generator_N_GF.hpp.in ${SRC_DIR_N_GF}/generator/dec_generator.hpp)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/apps/code_generator.cpp.in ${SRC_DIR_N_GF}/code_generator.cpp)

    add_executable (
      code_generator_N${N}_GF${GF} ${SRC_DIR_N_GF}/code_generator.cpp src/encoder/polar_encoder.cpp src/utilities/utility_functions.cpp
    )
    target_include_directories (code_generator_N${N}_GF${GF} PRIVATE ${SRC_DIR_N_GF} ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)

    foreach (Raw_R IN LISTS Rs)
      execute_process (
        COMMAND ${PYTHON_EXECUTABLE} -c "K = round(${N} * ${Raw_R}); print(K)" OUTPUT_VARIABLE K OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      execute_process (
        COMMAND ${PYTHON_EXECUTABLE} -c "K = round(${N} * ${Raw_R}); R = ${N} / ${K}; print(R)" OUTPUT_VARIABLE R
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )

      set (spec_benchmark_target benchmarking_dec${DEC_NO}_N${N}_GF${GF}_K${K})

      if (NOT TARGET ${spec_benchmark_target})
    	file (MAKE_DIRECTORY ${SRC_DIR_N_GF}/decoders/dedicated_K${K})
    	set (DEC_GENERATED_FILEPATH ${SRC_DIR_N_GF}/decoders/dedicated_K${K}/dedicated_execute.hpp)
    	configure_file (${CMAKE_CURRENT_SOURCE_DIR}/apps/benchmarking_decX_N_GF.cpp.in benchmarking_dec${DEC_NO}_N${N}_GF${GF}_K${K}.cpp)
    	configure_file (${CMAKE_CURRENT_SOURCE_DIR}/src/decoders/dedicated/decoder_dedicated.cpp.in ${SRC_DIR_N_GF}/decoders/dedicated_K${K}/decoder_dedicated.cpp)
        add_custom_command (
          OUTPUT ${DEC_GENERATED_FILEPATH} DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/generator/dec_generator_N_GF.hpp.in
		  COMMAND code_generator_N${N}_GF${GF} ARGS --K ${K} --path ${DEC_GENERATED_FILEPATH}
        )
        add_executable (
          ${spec_benchmark_target} benchmarking_dec${DEC_NO}_N${N}_GF${GF}_K${K}.cpp src/hadamard/Hadamard.cpp src/encoder/polar_encoder.cpp
                                   src/utilities/utility_functions.cpp
        )
        target_include_directories (${spec_benchmark_target} PUBLIC ${CMAKE_BINARY_DIR}/src_N${N}_GF${GF})

        add_custom_target (decoder_execute_N${N}_GF${GF}_K${K} DEPENDS ${DEC_GENERATED_FILEPATH})
        add_dependencies (${spec_benchmark_target} decoder_execute_N${N}_GF${GF}_K${K})
		target_sources (${spec_benchmark_target} PUBLIC ${SRC_DIR_N_GF}/decoders/dedicated_K${K}/decoder_dedicated.cpp)
        target_include_directories (${spec_benchmark_target} PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
        target_include_directories (${spec_benchmark_target} PUBLIC ${CMAKE_SOURCE_DIR}/src/decoders/dedicated)
      endif ()
    endforeach ()
  else ()
    set (spec_benchmark_target benchmarking_dec${DEC_NO}_N${N}_GF${GF})
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/src/definitions/code.hpp.in ${SRC_DIR_N_GF}/definitions/code.hpp)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/apps/benchmarking_decX_N_GF.cpp.in benchmarking_dec${DEC_NO}_N${N}_GF${GF}.cpp)

    add_executable (
      ${spec_benchmark_target} benchmarking_dec${DEC_NO}_N${N}_GF${GF}.cpp src/hadamard/Hadamard.cpp src/encoder/polar_encoder.cpp
                               src/utilities/utility_functions.cpp
    )
    target_include_directories (${spec_benchmark_target} PUBLIC ${CMAKE_BINARY_DIR}/src_N${N}_GF${GF})

    if (DEC_NO EQUAL 0)
      target_sources (${spec_benchmark_target} PUBLIC src/decoders/basic/decoder_basic.cpp)
    elseif (DEC_NO EQUAL 1)
      target_sources (${spec_benchmark_target} PUBLIC src/decoders/naive/decoder_naive.cpp)
    elseif (DEC_NO EQUAL 3)
      target_sources (${spec_benchmark_target} PUBLIC src/decoders/specialized/decoder_specialized.cpp)
    elseif (DEC_NO EQUAL 4)
      target_sources (${spec_benchmark_target} PUBLIC src/decoders/specialized_pruning/decoder_specialized_pruning.cpp)
    else ()
      message (FATAL_ERROR "Decoder type ${DEC_NO} is not defined correctly.")
    endif ()
    target_include_directories (${spec_benchmark_target} PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
    target_include_directories (${spec_benchmark_target} PUBLIC ${CMAKE_SOURCE_DIR}/src/decoders/dedicated)
  endif ()
endforeach ()
#
# src/decoders/naive_fixed/decoder_naive_fixed.cpp src/decoders/naive_int32_t/decoder_naive_int32_t.cpp
# src/decoders/naive_cfloat/decoder_naive_cfloat.cpp
# ##########################################################################################################################################

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3
# -ffast-math -g3")

add_executable (fwht_validator ${CMAKE_CURRENT_SOURCE_DIR}/apps/bench_fwht.cpp)
target_include_directories (fwht_validator PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries (fwht_validator PUBLIC utilities)

# ##########################################################################################################################################

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3
# -ffast-math -g3")

add_executable (code_generator ${CMAKE_CURRENT_SOURCE_DIR}/apps/code_generator.cpp src/encoder/polar_encoder.cpp)
target_include_directories (code_generator PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries (code_generator PUBLIC utilities)

# ##########################################################################################################################################

add_subdirectory (src)

# ##########################################################################################################################################

add_library (
  NbScFFTdec SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/apps/LibNbScFFTdec.cpp
  src/decoders/specialized_pruning/decoder_specialized_pruning.cpp
  src/decoders/specialized/decoder_specialized.cpp
  src/decoders/dedicated/decoder_dedicated.cpp
  src/decoders/naive/decoder_naive.cpp
  src/decoders/basic/decoder_basic.cpp
  src/decoders/naive_fixed/decoder_naive_fixed.cpp
  src/decoders/naive_int32_t/decoder_naive_int32_t.cpp
  src/decoders/naive_cfloat/decoder_naive_cfloat.cpp
  src/hadamard/Hadamard.cpp
  src/encoder/polar_encoder.cpp
)
target_compile_options (NbScFFTdec PUBLIC "$<$<CONFIG:Debug>:${GCC_COMPILE_DEBUG_OPTIONS}>" -DALL_GFs)
target_compile_options (NbScFFTdec PUBLIC "$<$<CONFIG:Release>:${GCC_COMPILE_RELEASE_OPTIONS}>" -DALL_GFs)
# add_compile_options (-DALL_GFs)
target_include_directories (NbScFFTdec PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries (NbScFFTdec PUBLIC utilities)

# ##########################################################################################################################################
