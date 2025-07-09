
add_library (utilities utilities/utility_functions.cpp)
target_include_directories (utilities PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_library (node-type utilities/NodeType.cpp)
target_include_directories (node-type PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_library (frozentree utilities/frozen_tree.cpp)
target_include_directories (frozentree PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries (frozentree PUBLIC node-type)

add_executable (frozen-tree-builder utilities/frozen_tree_builder.cpp)
target_link_libraries (frozen-tree-builder PRIVATE frozentree)

# -Wno-unknown-pragmas -Wno-gnu-inline-cpp-without-extern -march=native -mtune=native -O3

add_executable (bench_fwht features/fwht/bench_fwht.cpp)
target_include_directories (bench_fwht PUBLIC ${CMAKE_SOURCE_DIR}/src)

#add_executable (test_function_f ${CMAKE_CURRENT_SOURCE_DIR}/node_functions/test_function_f.cpp)
#target_include_directories (test_function_f PUBLIC ${CMAKE_SOURCE_DIR}/src)
#target_link_libraries (test_function_f PUBLIC utilities)

