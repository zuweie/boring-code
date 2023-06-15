/*
 * @Author: your name
 * @Date: 2020-11-03 08:33:31
 * @LastEditTime: 2020-11-30 12:59:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/test_mem_pool.c
 */
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "mem_pool/__mem_pool.h"
#include "test_data.h"
static int  suite_success_init (void) 
{
    printf("\n Memory pool suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n Memory pool suite success clean\n");
}

static void test_mempool_set_n_get_slot (void) 
{
    //pool_t* pool = alloc_create(0);
    //void* pmem = allocate(pool, 109);
    void* pmem = malloc(128);
    memset(pmem, 0, 128);
    size_t test_slot = 186;

    POOL_SET_SLOT(pmem, test_slot);
    int ret_slot = POOL_GET_SLOT(pmem);
    
    CU_ASSERT_TRUE(ret_slot<=__MAX_VAL_OF_BYTES(__SLOT_INFO_BYTES));
    CU_ASSERT_TRUE(ret_slot==test_slot);

    POOL_SET_SLOT(pmem, 257);
    ret_slot = POOL_GET_SLOT(pmem);
    CU_ASSERT_TRUE(ret_slot!=257);

    free(pmem);
}

int do_memory_pool_test(void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Memory pool Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
        
    if (NULL == CU_add_test(pSuite, "test memory pool set and get", test_mempool_set_n_get_slot) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

}