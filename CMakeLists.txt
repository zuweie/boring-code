cmake_minimum_required(VERSION 3.5)

project(boring-code-testcase)

set (SOURCE
    src/unit_test/unit_test.c
    src/unit_test/unit_test_tv.c
    src/unit_test/unit_test_vector.c
    src/unit_test/unit_test_list.c
    src/unit_test/unit_test_hashmap.c
    src/unit_test/test_data.c
    src/base/__list.c
    src/base/__rb_tree.c
    src/base/__vector.c
    src/base/__hashmap.c
    src/base/operate/__sort.c
    src/base/operate/__wring.c
    src/graph/graph_search.c
    src/graph/graph.c
    src/matrix/matrix.c
    src/matrix/tsmatrix.c
    src/mem_pool/__mem_pool.c
)

## 连接 cunit 库
set(INC_DIR 
    /usr/local/include/
    src/
)

set(LIB_DIR /usr/local/lib/)
include_directories(${INC_DIR})
link_directories(${LIB_DIR})

set(CMAKE_BUILD_TYPE "Debug")

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} cunit)