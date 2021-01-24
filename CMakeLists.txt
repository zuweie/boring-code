cmake_minimum_required(VERSION 3.5)

project(boring-code-testcase)

set (SOURCE
    src/unit_test/unit_test.c
    src/unit_test/unit_test_tv.c
    src/unit_test/unit_test_vector.c
    src/unit_test/unit_test_list.c
    src/unit_test/unit_test_hashmap.c
    src/unit_test/unit_test_mempool.c
    src/unit_test/unit_test_coo_matrix.c
    src/unit_test/unit_test_dense_matrix.c
    src/unit_test/unit_test_graph.c
    src/unit_test/unit_test_udgraph.c
    src/unit_test/unit_test_heap_sort.c
    src/unit_test/unit_test_set.c
    src/unit_test/unit_test_dft_fft.c
    src/unit_test/unit_test_dct.c
    src/unit_test/unit_test_asr.c
    src/unit_test/test_data.c
    src/base/__list.c
    src/base/__rb_tree.c
    src/base/__vector.c
    src/base/__hashmap.c
    src/base/operate/__sort.c
    src/base/operate/__wring.c
    src/base/operate/__heap_sort.c
    src/container/Entity.c
    src/graph/graph_research.c
    src/graph/graph.c
    src/graph/ud_graph.c
    src/matrix/CooMatrix.c
    src/matrix/DenseMatrix.c
    src/mem_pool/__mem_pool.c
    src/fourier_transform/fourier_transform.c
    src/cosine_transform/cosine_transform.c
    src/automatic_speech_recognition/mfcc.c
    src/automatic_speech_recognition/signal_process.c
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