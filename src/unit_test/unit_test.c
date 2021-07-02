/*
 * @Description: test case for unc
 * @Author: your name
 * @Date: 2019-09-04 10:43:36
 * @LastEditTime: 2021-07-02 14:30:21
 * @LastEditors: Please set LastEditors
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "mem_pool/__mem_pool.h"
#include "base/__type_value.h"
#include "base/__vector.h"
#include "base/__list.h"
#include "base/__rb_tree.h"
#include "base/operate/__sort.h"
#include "container/Cn.h"
#include "container/It.h"
#include "container/Tv.h"
#include "graph/graph.h"
#include "graph/graph_research.h"
#include "unit_test.h"
#include "test_data.h"

int main () 
{
    // init test data
    init_test_data();
       
    if (CUE_SUCCESS != CU_initialize_registry()){
	    return CU_get_error();
    }

    /* run the testcase */
    // do_vector_test();
    // do_list_test();
    // do_tv_test();
    // do_hashmap_test();
    // do_memory_pool_test();
    // do_coo_matrix_test();
    // do_dense_matrix_test();
    // do_graph_test();
    // do_udgraph_test();
    // do_set_test();
    // do_heapsort_test();
    // do_fft_test();
    // do_dct_test();
    // do_asr_test();
    // do_ultra_array_test();
    // do_macine_learning_test();
    // do_matrix_test();
    // do_toutiao_test();
    do_svm_test();
    /* run the test case */
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}