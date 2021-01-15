/*
 * @Description: test case for unc
 * @Author: your name
 * @Date: 2019-09-04 10:43:36
 * @LastEditTime: 2021-01-15 09:16:20
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

void 
test_mem_attr(void) {
    CU_ASSERT(__FREELIST_SIZE == 16);

    printf("%d", __FREELIST_SIZE);
    printf("\n%d\n", POOL_ROUND_UP(9));
    printf("%d\n", POOL_ROUND_UP(97));
    printf("%d\n", POOL_ROUND_UP(3399));

    printf("\n%d\n", sizeof(char));
    printf("%d\n", sizeof(int));
    printf("%d\n", sizeof(float));
    
    printf("------------free-index--------------------\n");
    printf("%d\n", POOL_FREELIST_INDEX(1));
    printf("%d\n", POOL_FREELIST_INDEX(2));
    printf("%d\n", POOL_FREELIST_INDEX(8));
    printf("%d\n", POOL_FREELIST_INDEX(9));
    printf("%d\n", POOL_FREELIST_INDEX(16));
    printf("%d\n", POOL_FREELIST_INDEX(17));
    printf("%d\n", POOL_FREELIST_INDEX(23));
    printf("%d\n", POOL_FREELIST_INDEX(24));
    printf("%d\n", POOL_FREELIST_INDEX(27));
    printf("%d\n", POOL_FREELIST_INDEX(28));
    printf("%d\n", POOL_FREELIST_INDEX(30));
    printf("%d\n", POOL_FREELIST_INDEX(32));
    printf("%d\n", POOL_FREELIST_INDEX(39));
    printf("%d\n", POOL_FREELIST_INDEX(128));
    CU_ASSERT(POOL_ROUND_UP(19) == 24);
}

void test_mem_pool_inspect (void) {
    // 这个他妈的是戏肉啊。
    CU_ASSERT(1);
    pool_t* pool = alloc_create(0);
    //alloc_init(&pool);
    //inspect_pool(&pool);
    char* test1 = allocate(pool, 25);
    char* test2 = allocate(pool, 8);
    char* test3 = allocate(pool, 9);
    char* test4 = allocate(pool, 17);
    char* test5 = allocate(pool, 25);
    char* test6 = allocate(pool, 33);
    char* test7 = allocate(pool, 2031);
    inspect_pool(pool);

    // memset(test1, 0, 25);
    // memset(test2, 0, 8);
    // memset(test3, 0, 9);
    // memset(test4, 0, 17);
    // memset(test5, 0, 25);
    // memset(test6,0, 33);

    deallocate(pool, test1);
    deallocate(pool, test2);
    deallocate(pool, test3);
    deallocate(pool, test4);
    deallocate(pool, test5);
    deallocate(pool, test6);
    deallocate(pool, test7);
    inspect_pool(pool);
    alloc_destroy(pool);
}

void 
test_mem_pool_maxslot (void)
{
    CU_ASSERT(1);
    printf("\n%ld\n", (unsigned int)__MAX_FREELIST_SIZE);
}




void test_rb_tree(void) 
{
    
    
    // //init_rb_tree(rbtree, compare_int, g_pool(0));
    // RbTree tree;
    // RbTree_init(tree, compare_int, compare_int);

    // for(int i=0; i<11; ++i) {
    //     CN_insert(tree, RbTree_root(tree), i2t(i));
    // }

    // /** 展示 **/
    // printf("\n size of tree: %d \n", CN_size(tree));
    // It first = CN_first(tree);
    // It tail = CN_tail(tree);
    // printf("list the element of tree: \n");
    // for(;!It_equal(first, tail); first = It_next(first))
    // {
    //     int v = type_int( It_dref(first) );
    //     printf(" %d ", v);
    // }

    /** 删除 **/
    // for(int i=0; i<TEST_DATA_SIZE/2; ++i) {
        
    //     iterator_t pos = container_find(rbtree, get(i), compare_int);

    //     int data =  container_remove(rbtree, pos, NULL);
        
    //     printf("\n delete %d \n", data);

    // }
    


    // /** 展示 **/
    // printf("\n size of tree: %d ", container_size(rbtree));
    // first = container_first(rbtree);
    // tail = iterator_next( container_last(rbtree) );
    // for(;!iterator_equal(first, tail); first = iterator_next(first))
    // {
    //     int v = type_int( iterator_dereference(first) );
    //     printf("\n %d \n", v);
    // }
    // container_destroy(rb_tree, rbtree);
    CU_ASSERT(1);
}

void test_graph () 
{

    // 生成一个定点加入图中
    // Graph graph;
    // Graph_init(&graph, find_vertex, find_edge);
    // // 添加定点
    // for(int i=0; i<10; ++i) {
    //     Graph_addVertex(&graph, getcc(i));
    // }

    // // 添加边
    // vertex_t* from = Graph_getVertex(&graph, getcc(3));
    // vertex_t* to   = Graph_getVertex(&graph, getcc(5));
    
    // if (from && to) {
    //     Graph_addEdge(from, to, 0.0);
    // }
    /*
    from = Graph_getVertex(&graph, getcc(5));
    to   = Graph_getVertex(&graph, getcc(8));

    if (from && to) {
        Graph_addEdge(&graph, from , to, 0.0);
    }

    from = Graph_getVertex(&graph, getcc(5));
    to   = Graph_getVertex(&graph, getcc(2));

    if (from && to) {
        Graph_addEdge(&graph, from , to, 0.0);
    }

    from = Graph_getVertex(&graph, getcc(5));
    to   = Graph_getVertex(&graph, getcc(3));

    if (from && to) {
        Graph_addEdge(&graph, from , to, 0.0);
    }

    from = Graph_getVertex(&graph, getcc(5));
    to   = Graph_getVertex(&graph, getcc(4));

    if (from && to) {
        Graph_addEdge(&graph, from , to, 0.0);
    }

    from = Graph_getVertex(&graph, getcc(5));
    to   = Graph_getVertex(&graph, getcc(6));

    if (from && to) {
        Graph_addEdge(&graph, from , to, 0.0);
    }

    from = Graph_getVertex(&graph, getcc(6));
    to   = Graph_getVertex(&graph, getcc(8));

    if (from && to) {    
        Graph_addEdge(&graph, from, to, 0.0);
    }

    from = Graph_getVertex(&graph, getcc(8));
    to   = Graph_getVertex(&graph, getcc(2));
    if (from && to) {    
        Graph_addEdge(&graph, from, to, 0.0);
    }

    from = Graph_getVertex(&graph, getcc(2));
    to   = Graph_getVertex(&graph, getcc(9));

    if (from && to) {    
        Graph_addEdge(&graph, from, to, 0.0);
    }
    
    printf("\n\n");

    vertex_t* start = Graph_getVertex(&graph, getcc(3));
    vertex_t* desc  = Graph_getVertex(&graph, getcc(9));
    grp_bfs(&graph, start);


    printf ("\n*************bfs***************\n");
    LinkArray arr;
    LinkArr_init(&arr, NULL);
    grp_bfs_path(&graph, start, desc, &arr);

    CN_inspect(&arr, VERTEX_CHAR);

    LinkArr_free(&arr);
    */
   
    //Matrix* matrix = Matrix_create(CN_size(&graph.vertexes), CN_size(&graph.vertexes));
    // size_t size = CN_size(graph.vertexes);
    // TSMatrix* tsmatrix = TSMatrix_create(size, size);

    // Graph_getEdgeMatrix(&graph, tsmatrix);

    // TSMatrix_set(tsmatrix, 1, 2, 1.0);
    // Graph_addEdgeByMatrix(&graph, tsmatrix, 0.f);
    
    // //Matrix_inspect(matrix);
    // Graph_inspect(&graph, PRINTF_TV_ON_CHAR);
    // TSMatrix* transpose = TSMatrix_create_transpose(tsmatrix);
    // Graph_addEdgeByMatrix(&graph, transpose, 0.f);
    // Graph_inspect(&graph, PRINTF_TV_ON_CHAR);

    // TSMatrix_destroy(tsmatrix);
    // TSMatrix_destroy(transpose);
    // Graph_free(&graph);
    
    // CU_ASSERT(1);
}

void test_matrix()
{
    /*
    float data[6][4] = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12},
        {13,14,15,16},
        {17,18,19,20},
        {21,22,23,24}
    };

    float data2 [1][8] = {
        {1,2,3,4,5,6,7,8}
    };
    Matrix* matrix1 = Matrix_create_by(1, 8, data2);
    Matrix_inspect(matrix1);
    Matrix* matrix2 = Matrix_create_transpose(matrix1);
    Matrix_inspect(matrix2);
    Matrix* matrix3 = Matrix_create(4,4);
    Matrix_inspect(matrix3);
    */

    // Matrix* matrix4 = Matrix_create(4,2);
    // Matrix_inspect(matrix4);
    // Matrix_set(matrix4, 0,1, 1.2);
    // Matrix_set(matrix4, 1,0, 2.3);
    // Matrix_set(matrix4, 3,1,4.5);
    // Matrix_inspect(matrix4);
    // Matrix_destroy(matrix4);

}

int main () 
{
    // init test data
    init_test_data();
       
    if (CUE_SUCCESS != CU_initialize_registry()){
	    return CU_get_error();
    }

    /* run the testcase */
    do_vector_test();
    do_list_test();
    do_tv_test();
    do_hashmap_test();
    do_memory_pool_test();
    do_coo_matrix_test();
    do_dense_matrix_test();
    do_graph_test();
    do_udgraph_test();
    do_set_test();
    do_heapsort_test();
    do_fft_test();
    do_dct_test();
    /* run the test case */
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}