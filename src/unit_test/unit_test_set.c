/*
 * @Author: your name
 * @Date: 2020-12-04 07:26:51
 * @LastEditTime: 2025-06-02 08:21:20
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_set.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>

#include "container/cn.h"
// #include "container/HashSet.h"
// #include "container/TreeSet.h"

#include "test_data.h"
static int  suite_success_init (void) 
{
    printf("\n Set suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n Set suite success clean\n");
}

static void test_hashset_set(void)
{
    //Set hashset = _Hashset(entity_int_keyhasher);
    CN hashset = CN_create(HASH_SET, int_t);

    for(int i=0; i<10; ++i) {
        CN_set(hashset, tsd_get_int(i));
    }

    for (int j=9; j>=0; j--) {
        CU_ASSERT_TRUE( CN_has(hashset, tsd_get_int(j)) );
    }
    CN_finalize(hashset, NULL);
}

static void test_hashset_del(void) 
{
    CN hashset = CN_create(HASH_SET, int_t);
    for (int i=0; i<10; ++i) {
        CN_set(hashset, tsd_get_int(i));
    }

    for (int j=3; j<7; ++j) {
        CN_del(hashset, tsd_get_int(j));
    }
 
    for (int k=3; k<7; ++k) {
        CU_ASSERT_FALSE(CN_has(hashset,tsd_get_int(k)));
    }

    CN_finalize(hashset, NULL);
}

static void test_treeset_set(void)
{
    CN treeset = CN_create(TREE_SET, int_t);
    for(int i=0; i<10; ++i) {
        CN_set(treeset, tsd_get_int(i));
    }

    for (int j=9; j>=0; j--) {
        CU_ASSERT_TRUE(CN_has(treeset, tsd_get_int(j)));
    }
    CN_finalize(treeset, NULL);
}

static void test_treeset_del(void)
{
    CN treeset = CN_create(TREE_SET, int_t);
    for (int i=0; i<10; ++i) {
        CN_set(treeset, tsd_get_int(i));
    }

    for (int j=3; j<7; ++j) {
        CN_del(treeset, tsd_get_int(j));
    }
 
    for (int k=3; k<7; ++k) {
        CU_ASSERT_FALSE(CN_has(treeset, tsd_get_int(k)));
    }

    CN_finalize(treeset, NULL);
}
int do_set_test(void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The Set Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test hashset set", test_hashset_set) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test hashset del", test_hashset_del) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test treeset set", test_treeset_set) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

        
    if (NULL == CU_add_test(pSuite, "test treeset del", test_treeset_del) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}