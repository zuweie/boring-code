/*
 * @Author: your name
 * @Date: 2020-12-04 07:26:51
 * @LastEditTime: 2021-06-29 15:10:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_set.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/HashSet.h"
#include "container/TreeSet.h"
#include "cmp_component.h"
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
    Set hashset = _Hashset(entity_int_keyhasher);
    for(int i=0; i<10; ++i) {
        Set_set(&hashset, getTSi(i));
    }

    for (int j=9; j>=0; j--) {
        CU_ASSERT_TRUE(Set_has(&hashset, getTSi(j)));
    }
    Hashset_(&hashset);
}

static void test_hashset_del(void) 
{
    Set hashset = _Hashset(entity_int_keyhasher);
    for (int i=0; i<10; ++i) {
        Set_set(&hashset, getTSi(i));
    }

    for (int j=3; j<7; ++j) {
        Set_del(&hashset, getTSi(j), NULL);
    }
 
    for (int k=3; k<7; ++k) {
        CU_ASSERT_FALSE(Set_has(&hashset, getTSi(k)));
    }

    Hashset_(&hashset);
}

static void test_treeset_set(void)
{
    Set treeset = _Treeset(entity_int_insert_cmp);
    for(int i=0; i<10; ++i) {
        Set_set(&treeset, getTSi(i));
    }

    for (int j=9; j>=0; j--) {
        CU_ASSERT_TRUE(Set_has(&treeset, getTSi(j)));
    }
    Treeset_(&treeset);
}

static void test_treeset_del(void)
{
    Set treeset = _Treeset(entity_int_keyhasher);
    for (int i=0; i<10; ++i) {
        Set_set(&treeset, getTSi(i));
    }

    for (int j=3; j<7; ++j) {
        Set_del(&treeset, getTSi(j), NULL);
    }
 
    for (int k=3; k<7; ++k) {
        CU_ASSERT_FALSE(Set_has(&treeset, getTSi(k)));
    }

    Treeset_(&treeset);
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