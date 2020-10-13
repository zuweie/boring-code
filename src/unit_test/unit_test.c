/*
 * @Description: test case for unc
 * @Author: your name
 * @Date: 2019-09-04 10:43:36
 * @LastEditTime: 2020-10-13 07:04:44
 * @LastEditors: Please set LastEditors
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "mem_pool/__mem_pool.h"
#include "base/__type_value.h"
#include "base/__vector.h"
#include "base/__list.h"
#include "base/__rb_tree.h"
#include "base/__sort.h"
#include "container/cn.h"
#include "container/it.h"
#include "container/tv.h"
#include "graph/graph.h"
#include "graph/graph_search.h"
#include "matrix/matrix.h"
#include "container/RbTree.h"
#include "unit_test.h"

#define TEST_DATA_SIZE 10000
#define PRINTF_TV_ON_INT(tv) printf("%d ", t2i(tv))
#define PRINTF_TV_ON_CHAR(tv) printf("%c ", t2i(tv))
#define PRINTF_IT_ON_INT(it) PRINTF_TV_ON_CHAR(It_dref(it))
#define PRINTF_IT_ON_CHAR(it) PRINTF_TV_ON_CHAR(It_dref(it))
#define PRINTF_IT_ON_VERTEX_CHAR(it) do{ \
    vertex_t* v = It_getptr(it); \
    PRINTF_TV_ON_CHAR(v->vertex_id);\
    printf(" -- %d -> ", ((bfs_explor_t*)v->exploring)->distance);\
    }while(0) 
int suite_success_init(void) {
    return 0;
}

int suite_success_clean(void) {
    return 0;
}

static type_value_t test_data[TEST_DATA_SIZE];
static void init_test_data()
{
    for(int i=0; i<TEST_DATA_SIZE; ++i) {
        test_data[i] = int_type(rand() % 100000);
    }
    return;
}
static Tv get(int i) {
    return test_data[i];
}
static Tv getcc(int i) {
    char* abc = "abcdefghijklmnopqrstuvwxyz";
    if (i<26 && i >=0) {
        return i2t(abc[i]);
    }else {
        char e = 'E';
        return i2t(e);
    }
}


#define CN_inspect(con, type)  do{ \
    printf(" ********* inspection of container *****************\n"); \
    CN_travel(con, PRINTF_IT_ON_##type); \
    printf("\n"); \
}while(0)


#define Graph_inspect(graph, printer) do{ \
    printf(" ********* inspection of Graph *****************\n"); \
    for (It i = CN_first( ((graph)->vertexes) ); !It_equal(i, CN_tail( ((graph)->vertexes) ) ); i = It_next(i)) { \
        vertex_t* pv = It_getptr(i); \
        printf("vertex: "); \
        printer(pv->vertex_id); \
        printf("(%d) ", pv->indexing); \
        printf("------> "); \
        for (It j = CN_first(pv->edges); !It_equal(j, CN_tail(pv->edges)); j = It_next(j)) { \
            edge_t* pnode = It_getptr(j); \
            printer(pnode->to->vertex_id); \
            printf("(%d)   ", pnode->to->indexing);\
        }\
        printf("\n\n"); \
    } \
}while(0)

#define Matrix_inspect(matrix) do{ \
    printf(" ************* inspection of matrix ********************** \n");\
    float (*pdata)[(matrix)->col] = (matrix)->elems;\
    for (int i=0; i<(matrix)->row; ++i) { \
        if (i == 0) { \
            printf("   ");\
            for (int h=0; h<(matrix)->col; ++h) { \
                printf("%4d  ", h); \
            } \
            printf("\n"); \
        }\
        for (int j=0; j<(matrix)->col; ++j) { \
            if (j == 0) { \
                printf("%2d ", i); \
            } \
            printf("%.2f  ", Matrix_data(matrix)[i][j]);\
        } \
        printf("\n"); \
    } \
}while(0)

int find_vertex(Tv v1, Tv v2) 
{
    vertex_t* pv = t2p(v1);
    return compare_int(pv->vertex_id, v2);
}

int find_edge(Tv v1, Tv v2) 
{
    edge_t* pl = t2p(v1);
    return compare_int(pl->to->vertex_id, v2);
}

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

void test_vector (void) {
    vector_t* vet = container_create(vector);
    container_insert(vet, container_tail(vet), int_type(1));
    container_insert(vet, container_tail(vet), int_type(2));
    
    for(iterator_t first = container_first(vet); 
        !iterator_equal(first, container_tail(vet)); 
        first=iterator_next(first)) {

            printf("%d ", type_int(iterator_dereference(first)));

    }
    
    printf("\n***********************\n");
    
    container_insert_find(vet, int_type(2), int_type(3), compare_int);
    container_insert_find(vet, int_type(3), int_type(4), compare_int);
    container_insert_find(vet, int_type(5), int_type(9), compare_int);
    container_insert_find(vet, int_type(78), int_type(3), compare_int);
    for(iterator_t first = container_first(vet); 
        !iterator_equal(first, container_tail(vet)); 
        first=iterator_next(first)) {

            printf("%d ", type_int(iterator_dereference(first)));

    }
    printf("\n***********************\n");
    int result = container_remove(vet, container_tail(vet), NULL);
    
    
    for(iterator_t first = container_first(vet); 
        !iterator_equal(first, container_tail(vet)); 
        first=iterator_next(first)) {

            printf("%d ", type_int(iterator_dereference(first)));

    }

    CU_ASSERT((result == -1));
    printf("\n***********************\n");
    result = container_remove_find(vet, int_type(3), NULL, compare_int);
    for(iterator_t first = container_first(vet); 
        !iterator_equal(first, container_tail(vet)); 
        first=iterator_next(first)) {

            printf("%d ", type_int(iterator_dereference(first)));

    }
    CU_ASSERT((result == 0));
    printf("\n***********************\n");
    result = container_remove_find(vet, int_type(10), NULL, compare_int);
    for(iterator_t first = container_first(vet); 
        !iterator_equal(first, container_tail(vet)); 
        first=iterator_next(first)) {

            printf("%d ", type_int(iterator_dereference(first)));

    }
    container_destroy(vector, vet);
    CU_ASSERT((result == 0));

}

void test_list (void) {

    list_t* list = container_create(list);
    //init_list(list, g_pool(0));

    printf("\n********** size fo type_value_t %d *********************\n", sizeof(type_value_t));

    for(int i=0; i<40; ++i) {
        container_insert(list, container_first(list), get(i));
    }

    iterator_t tail = container_tail(list);
    iterator_t first = container_first(list);
    
    printf("\n********* before sort ***********\n");
    for(; !iterator_equal(first, tail); first = iterator_next(first)) {
        int v = type_int( iterator_dereference(first) );
        printf("%d ", v);
    }

    //quick_sort(container_first(list), container_last(list), compare_int);
    container_sort(list, compare_int);
    
    tail = container_tail(list);
    first = container_first(list);
    printf("\n******** after sort ***************\n");
    for(; !iterator_equal(first, tail); first = iterator_next(first)) {
        int v = type_int( iterator_dereference(first) );
        printf("%d ", v);
    }
    printf("\n************ inspact pool *******************\n");
    inspect_pool(container_mem_pool(list));
    
    container_destroy(list, list);
    CU_ASSERT(1);
}

void test_rb_tree(void) 
{
    
    
    //init_rb_tree(rbtree, compare_int, g_pool(0));
    RbTree tree;
    RbTree_init(tree, compare_int, compare_int);

    for(int i=0; i<11; ++i) {
        CN_insert(tree, RbTree_root(tree), i2t(i));
    }

    /** 展示 **/
    printf("\n size of tree: %d \n", CN_size(tree));
    It first = CN_first(tree);
    It tail = CN_tail(tree);
    printf("list the element of tree: \n");
    for(;!It_equal(first, tail); first = It_next(first))
    {
        int v = type_int( It_dref(first) );
        printf(" %d ", v);
    }

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
    Graph graph;
    Graph_init(&graph, find_vertex, find_edge);
    // 添加定点
    for(int i=0; i<10; ++i) {
        Graph_addVertex(&graph, getcc(i));
    }

    // 添加边
    vertex_t* from = Graph_getVertex(&graph, getcc(3));
    vertex_t* to   = Graph_getVertex(&graph, getcc(5));
    
    if (from && to) {
        Graph_addEdge(from, to, 0.0);
    }
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
    size_t size = CN_size(graph.vertexes);
    TSMatrix* tsmatrix = TSMatrix_create(size, size);

    Graph_getEdgeMatrix(&graph, tsmatrix);

    TSMatrix_set(tsmatrix, 1, 2, 1.0);
    Graph_addEdgeByMatrix(&graph, tsmatrix, 0.f);
    
    //Matrix_inspect(matrix);
    Graph_inspect(&graph, PRINTF_TV_ON_CHAR);
    TSMatrix* transpose = TSMatrix_create_transpose(tsmatrix);
    Graph_addEdgeByMatrix(&graph, transpose, 0.f);
    Graph_inspect(&graph, PRINTF_TV_ON_CHAR);

    TSMatrix_destroy(tsmatrix);
    TSMatrix_destroy(transpose);
    Graph_free(&graph);
    
    CU_ASSERT(1);
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

    Matrix* matrix4 = Matrix_create(4,2);
    Matrix_inspect(matrix4);
    Matrix_set(matrix4, 0,1, 1.2);
    Matrix_set(matrix4, 1,0, 2.3);
    Matrix_set(matrix4, 3,1,4.5);
    Matrix_inspect(matrix4);
    Matrix_destroy(matrix4);

}

int main () 
{
    printf("test unc what ");
    init_test_data();

    
    if (CUE_SUCCESS != CU_initialize_registry()){
	    return CU_get_error();
    }
    
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("big test", suite_success_init, suite_success_clean);

    if (NULL == pSuite){
      CU_cleanup_registry();
      return CU_get_error();
    } 

    do_vector_test();
    
    // if (NULL == CU_add_test(pSuite, "test_vector", test_vector) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    
    
    // if (NULL == CU_add_test(pSuite, "test_rb_tree", test_rb_tree) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    
    
    // if (NULL == CU_add_test(pSuite, "test_list", test_list) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    
    // if (NULL == CU_add_test(pSuite, "test_graph", test_graph))
    // {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    
    
    // if (NULL == CU_add_test(pSuite, "test_matrix", test_matrix))
    // {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test_mem_pool_maxslot", test_mem_pool_maxslot) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test_mem_pool_inspect", test_mem_pool_inspect) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    

    // if (NULL == CU_add_test(pSuite, "test_mem_attr", test_mem_attr)) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
   
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}