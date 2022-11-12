cmake_minimum_required(VERSION 3.5)

project(boring-code-testcase)

set (SOURCE
    src/unit_test/unit_test.c
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
    src/unit_test/unit_test_ultra_array.c
    src/unit_test/unit_test_machine_learning.c
    src/unit_test/unit_test_matrix.c
    src/unit_test/unit_test_toutiao.c
    src/unit_test/unit_test_t.c
    src/unit_test/test_data.c
    src/unit_test/unit_test_svm.c
    src/unit_test/unit_test_neural_network.c
    src/unit_test/unit_test_decision_tree.c
    src/unit_test/unit_test_leetcode.c
    src/base/__list.c
    src/base/__rb_tree.c
    src/base/__vector.c
    src/base/__hash.c
    src/base/operate/__quick_sort.c
    src/base/operate/__wring.c
    src/base/operate/__heap_sort.c
    src/base/type_value/__built_in_type_adapters.c
    src/base/type_value/__type_value_def.c
    src/container/entity.c
    src/container/cn.c
    src/graph/graph_research.c
    src/graph/graph.c
    src/graph/ud_graph.c
    src/matrix/CooMatrix.c
    src/matrix/DenseMatrix.c
    src/matrix/matrix.c
    src/mem_pool/__mem_pool.c
    src/fourier_transform/fourier_transform.c
    src/cosine_transform/cosine_transform.c
    src/automatic_speech_recognition/mfcc.c
    src/automatic_speech_recognition/signal_process.c
    src/automatic_speech_recognition/simple_wav.c
    src/automatic_speech_recognition/compare_mfcc.c
    src/ultra_array/ultra_array.c
    src/ultra_array/ultra_router.c
    src/ultra_array/ultra_data_chunk.c
    src/ultra_array/ultra_padding.c
    src/machine_learning/linear_regression.c
    src/machine_learning/logistic_regression.c
    src/machine_learning/svm/support_vector_machines.c
    src/machine_learning/svm/solver.c
    src/machine_learning/svm/svm_model.c
    src/machine_learning/svm/svm_kernel_function.c
    src/machine_learning/neural_network.c
    src/machine_learning/decision_tree.c
    src/leetcode/lcs.c
    src/leetcode/kmp.c
    src/leetcode/regexp.c
    src/leetcode/longest_palindrome.c
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