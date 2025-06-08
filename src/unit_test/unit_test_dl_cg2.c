/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-31 22:44:25
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-08 13:51:01
 * @FilePath: /boring-code/src/unit_test/unit_test_dl_cg2.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
// 计算图2 测试用例

#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "unit_test.h"
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_hash.h"
#include "deep_learning/compute_graph2/cg_graph.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/compute_graph2/cg.h"


static int key_hash(void* key) 
{
    unsigned int hash = 0;
    const char* str = key;
    while(*str) {
        hash = (hash * 31 + *str) % SLOT_NUM;
        str++;
    }
    return hash;
}
static int key_cmp(void* k1, void* k2) 
{
    return strcmp(k1, k2);
}

static void print_path(const char* path_name, cg_list_t* path) 
{
    printf("%s:", path_name);

    cg_node_t* first = CG_LIST_TOP(path);

    while(first != CG_LIST_HEAD(path)) {

        cg_vertex_t* vertex = first->ref;
        if(first->prev == CG_LIST_HEAD(path))
            printf("(%s(eop))", vertex->id);
        else
            printf("(%s) --> ", vertex->id);

        first = first->prev;
    }
    printf("\n");
    return;
}

static void print_paths(const char* paths_name, cg_list_t* paths) 
{
    cg_node_t* first = CG_LIST_TOP(paths);

    while(first != CG_LIST_HEAD(paths)) {

        print_path(paths_name, first->ref);
        first = first->prev;
    }
    
    printf("\n\n");
    return;
}

static int recycle_path(cg_ref_t path) 
{
    cg_list_t* list = path;
    cg_list_recycle(list, NULL);
    return 0;
}

static int  suite_success_init (void) 
{
    printf("\ncg2 suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\ncg2 suite success clean\n");
}

static void cg2_testcase(void) 
{
    printf("cg2 test case\n");
}

static void cg_list_testcase(void) 
{
    char* test_str[10] = {"hello", "I", "am", "Jake", "and", "What`s", "up", "monther", "fucker", "?"};
    cg_list_t* list = cg_list_create();
    CU_ASSERT_TRUE(cg_list_is_empty(list));

    for (int i=0; i<10; ++i) {
        cg_list_push(list, test_str[i]);
    }
    int list_number = 0;
    cg_node_t* first = CG_LIST_TOP(list);
    CU_ASSERT_STRING_EQUAL(first->ref, "?");
    CU_ASSERT_STRING_EQUAL(first->prev->ref, "fucker");
    CU_ASSERT_STRING_EQUAL(first->prev->prev->ref, "monther");

    // for (;first != CG_LIST_HEAD(list); first = first->prev) {
    //     list_number++;
    //     printf("%s ", first->ref);
    // }
    // printf("\n");

    cg_list_revert(list);
    first = CG_LIST_TOP(list);
    CU_ASSERT_STRING_EQUAL(first->ref, "hello");
    CU_ASSERT_STRING_EQUAL(first->prev->ref, "I");
    CU_ASSERT_STRING_EQUAL(first->prev->prev->ref, "am");

    cg_list_t* list_cpy = cg_list_create_cpy(list);
    first = CG_LIST_TOP(list_cpy);
    
    CU_ASSERT_STRING_EQUAL(first->ref, "hello");
    CU_ASSERT_STRING_EQUAL(first->prev->ref, "I");
    CU_ASSERT_STRING_EQUAL(first->prev->prev->ref, "am");

    char* str = cg_list_pop(list_cpy);
    first = CG_LIST_TOP(list_cpy);

    CU_ASSERT_STRING_EQUAL(str, "hello");
    CU_ASSERT_STRING_EQUAL(first->ref, "I");
    CU_ASSERT_STRING_EQUAL(first->prev->ref, "am");

    cg_list_recycle(list, NULL);
    cg_list_recycle(list_cpy, NULL);
    return;
}


static void cg_hash_testcase(void)
{
    char *key[11] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"};
    char *val[11] = {"11", "22", "33", "44", "55", "66", "77", "88", "99", "1010", "1111"};
    
    cg_hash_t* hash = cg_hash_create(key_hash, key_cmp);
    for (int i=0; i<11; ++i) {
        cg_hash_set(hash, key[i], val[i]);
    }

    cg_hash_node_t* first = CG_HASH_FIRST(hash);
    // printf("\n");
    // for (; first != CG_HASH_TAIL(hash); first=first->next) {
    //     printf("code: %d, key: %s, val: %s\n", first->hash_code, first->key, first->ref);
    // }
    // printf("\n");
    // for (int i=0; i<SLOT_NUM; ++i) {
    //     cg_hash_node_t* node = hash->slot[i];
    //     if (node) {
    //         printf("hash->slot[%d], code: %d, key: %s, val: %s\n", i, node->hash_code, node->key, node->ref);
    //     } else {
    //         printf("hash->slot[%d] is NULL\n", i);
    //     }
    // }

    char* str =  cg_hash_get(hash, "5");
    CU_ASSERT_STRING_EQUAL(str, "55")
    
    str       = cg_hash_get(hash, "11");
    CU_ASSERT_STRING_EQUAL(str, "1111");

    str       = cg_hash_del(hash, "8");
    CU_ASSERT_STRING_EQUAL(str, "88")

    CU_ASSERT_FALSE(cg_hash_has(hash, "8"));

    cg_hash_del(hash, "10");
    first = CG_HASH_FIRST(hash);

    // printf("\n");
    // for (; first != CG_HASH_TAIL(hash); first=first->next) {
    //     printf("code: %d, key: %s, val: %s\n", first->hash_code, first->key, first->ref);
    // }
    // printf("\n");
    // for (int i=0; i<SLOT_NUM; ++i) {
    //     cg_hash_node_t* node = hash->slot[i];
    //     if (node) {
    //         printf("hash->slot[%d], code: %d, key: %s, val: %s\n", i, node->hash_code, node->key, node->ref);
    //     } else {
    //         printf("hash->slot[%d] is NULL\n", i);
    //     }
    // }

    cg_hash_recycle(hash, NULL);
    return;
}

static void cg_allocator_testcase(void) 
{
    cg_allocator_t alloc;
    cg_allocator_init(&alloc);


    int alloc_size    = 88;
    int round_up_size = ROUND_UP(88+1);

    void* block = cg_alloc(&alloc, alloc_size);

    //printf("index: %d\n", BLOCK_INDEX(alloc_size));
    CU_ASSERT_EQUAL(BLOCK_INDEX(round_up_size), 11);

    CU_ASSERT_NOT_EQUAL(alloc.blocks[BLOCK_INDEX(round_up_size)], NULL);

    int block_count = 0;
    block_node_t* block_next = alloc.blocks[BLOCK_INDEX(round_up_size)];

    while (block_next)
    {
        block_next = block_next->link;
        block_count += 1;
    }
    
    CU_ASSERT_EQUAL(block_count, 63);

    cg_recycle(&alloc, block);

    block_next = alloc.blocks[BLOCK_INDEX(round_up_size)];
    block_count = 0;
    while (block_next)
    {
        block_next = block_next->link;
        block_count += 1;
    }

    CU_ASSERT_EQUAL(block_count, 64);

    CU_ASSERT_EQUAL(alloc.blocks[BLOCK_INDEX(round_up_size)], COVER(block));

    cg_allocator_reset(&alloc);

    return;
}

static void cg_tensor_testcase(void)
{
    cg_allocator_t alloc;
    cg_allocator_init(&alloc);
    
    cg_tensor_t* t1 = cg_tensor_create(&alloc, 3, 2, 3, 4);

    CU_ASSERT_EQUAL(TENSOR_AXES(t1), 3);
    CU_ASSERT_EQUAL(TENSOR_DIMEN(t1, 0), 2);
    CU_ASSERT_EQUAL(TENSOR_DIMEN(t1, 1), 3);
    CU_ASSERT_EQUAL(TENSOR_DIMEN(t1, 2), 4);


    CU_ASSERT_EQUAL(TENSOR_STRIDE(t1, 0), 12);
    CU_ASSERT_EQUAL(TENSOR_STRIDE(t1, 1), 4);
    CU_ASSERT_EQUAL(TENSOR_STRIDE(t1, 2), 1);

    CU_ASSERT_EQUAL(TENSOR_NUM(t1), 24);

    // cg_tensor_fill(t1,  1.0);
    // cg_tensor_inspect(t1);

    cg_tensor_arange(t1, 0, 24);
    //cg_tensor_inspect(t1);

    cg_tensor_t* t_slice = cg_tensor_slice(t1, 3, 1, 2, 1, 3, 2, 4);
    //cg_tensor_inspect(t_slice);

    cg_tensor_t* t_padding = cg_tensor_padding(t1, 3.14, 3, 1, 0, 2, 2, 1, 1);
    //cg_tensor_inspect(t_padding);
    
    float* v = cg_tensor_get(t1, 1, 1,2);
    CU_ASSERT_DOUBLE_EQUAL(*v, 18, 0.001);

    v = cg_tensor_get(t1, 0, 2, 3);
    CU_ASSERT_DOUBLE_EQUAL(*v, 11, 0.001);

    v = cg_tensor_get(t_slice, 0, 1, 0);
    CU_ASSERT_DOUBLE_EQUAL(*v, 22, 0.001);

    v = cg_tensor_get(t_padding, 2, 3, 1);
    CU_ASSERT_DOUBLE_EQUAL(*v, 16, 0.001);

    cg_tensor_t* tt = cg_tensor_create(&alloc, 3, 2, 2, 3);
    cg_tensor_arange(tt, 0, 2*2*3);
    cg_tensor_inspect(tt);

    // cg_tensor_t* cpy = cg_tensor_create_cpy(tt);
    // cg_tensor_inspect(cpy);

    cg_tensor_T(tt, 2, 1, 0);
    cg_tensor_inspect(tt);
    
    cg_tensor_recycle(t1);
    cg_tensor_recycle(t_slice);
    cg_tensor_recycle(t_padding);
    cg_tensor_recycle(tt);
    //cg_tensor_recycle(cpy);

    cg_allocator_reset(&alloc);
}

static void cg_graph_testcase(void)
{
    printf("\n");

    cg_graph_t graph;
    cg_graph_init(&graph);
    
    cg_vertex_t vertexes[10];
    for (int i=0; i<10; ++i) {
        sprintf(vertexes[i].id, "vtx_%d", i);
        vertexes[i].in_vertexes  = cg_list_create();
        vertexes[i].out_vertexes = cg_list_create();

        cg_graph_add_vertex(&graph, &vertexes[i]);
    }

    cg_graph_link(&vertexes[8], &vertexes[0]);
    cg_graph_link(&vertexes[0], &vertexes[2]);
    cg_graph_link(&vertexes[2], &vertexes[3]);
    cg_graph_link(&vertexes[3], &vertexes[5]);
    cg_graph_link(&vertexes[5], &vertexes[7]);

    cg_graph_link(&vertexes[0], &vertexes[4]);
    cg_graph_link(&vertexes[4], &vertexes[5]);

    cg_graph_link(&vertexes[8], &vertexes[1]);
    cg_graph_link(&vertexes[1], &vertexes[2]);

    cg_graph_link(&vertexes[1], &vertexes[6]);
    cg_graph_link(&vertexes[6], &vertexes[7]);
    cg_graph_link(&vertexes[9], &vertexes[1]);


    cg_list_t* paths_0_7 = cg_list_create();
    cg_graph_search_paths(&graph, &vertexes[0], &vertexes[7], paths_0_7);

    cg_list_t* paths_8_7 = cg_list_create();
    cg_graph_search_paths(&graph, &vertexes[8], &vertexes[7], paths_8_7);

    cg_list_t* paths_9_7 = cg_list_create();
    cg_graph_search_paths(&graph, &vertexes[9], &vertexes[7], paths_9_7);

    //print_paths("[paths: 0 to 7]", paths_0_7);
    //print_paths("[paths: 8 to 7]", paths_8_7);
    //print_paths("[paths: 9 to 7]", paths_9_7);

    cg_node_t* first = CG_LIST_TOP(paths_8_7);
    cg_node_t* _first = CG_LIST_TOP((cg_list_t*)first->ref);

    cg_vertex_t* vtx = _first->ref;
    CU_ASSERT_STRING_EQUAL(vtx->id, "vtx_8");
    vtx = _first->prev->ref;
    CU_ASSERT_STRING_EQUAL(vtx->id, "vtx_0");
    vtx = _first->prev->prev->ref;
    CU_ASSERT_STRING_EQUAL(vtx->id, "vtx_2");
    vtx = _first->prev->prev->prev->ref;
    CU_ASSERT_STRING_EQUAL(vtx->id, "vtx_3");
    vtx = _first->prev->prev->prev->prev->ref;
    CU_ASSERT_STRING_EQUAL(vtx->id, "vtx_5");
    vtx = _first->prev->prev->prev->prev->prev->ref;
    CU_ASSERT_STRING_EQUAL(vtx->id, "vtx_7");

    cg_list_recycle(paths_0_7, recycle_path);
    cg_list_recycle(paths_8_7, recycle_path);
    cg_list_recycle(paths_9_7, recycle_path);

    cg_graph_reset(&graph);

    return;
    
}



int do_cg2_test (void) 
{

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The cg2 Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    #if 0
    if (NULL == CU_add_test(pSuite, "test cg2 ..\n", cg2_testcase) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    #endif

    #if 0
    if (NULL == CU_add_test(pSuite, "test cg list ..\n", cg_list_testcase) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    #endif
    
    #if 0
    if (NULL == CU_add_test(pSuite, "test cg hash ..\n", cg_hash_testcase) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    #endif
    
    #if 0
    if (NULL == CU_add_test(pSuite, "test cg alloc ..\n", cg_allocator_testcase) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    #endif  

    #if 1
    if (NULL == CU_add_test(pSuite, "test cg tensor ..\n", cg_tensor_testcase) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    #endif

    #if 0
    if (NULL == CU_add_test(pSuite, "\ntest cg path ..\n\n", cg_graph_testcase) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    #endif

    return 0;
}