/*
 * @Author: your name
 * @Date: 2020-10-14 21:29:01
 * @LastEditTime: 2020-10-17 00:16:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_hashmap.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/Hashmap.h"
#include "container/Cn.h"
#include "unit_test/unit_test.h"
#include "unit_test/test_data.h"

#define PRINTF_HASH_NODE(it) \
    ({                                         \
        hash_node_t* pnode = It_getptr(it);    \
        PRINTF_TV_ON_X(pnode->entity.key, key, %2d, t2i);     \
        PRINTF_TV_ON_X(pnode->entity.value, value, %s, t2p); \
        printf("index: %2d ", pnode->slot_index);             \
        printf("\n");                                       \
    })
    
static int  suite_success_init (void) 
{
    printf("\nHashmap suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nHashmap suite success clean\n");
}

static void test_hashmap_set (void) 
{
    const int HASHMAP_SIZE = 200;
    Hashmap hashmap;
    Hashmap_init(hashmap, hashmap_ikey_hasher, EQUL);

    Tv key, value;
    Container hashtab;
    for(int i=0; i<HASHMAP_SIZE; ++i) {
        Tv key = getTSi(i);
        Tv value = getTSs(i);
        Hashmap_set(hashmap, key, value);
    }
    printf("\n\n");
    hashtab = Hashmap_table(hashmap);
    CN_travel(hashtab, PRINTF_HASH_NODE);
    printf("\n\n");
    
    // key   = i2t(1);
    // value = p2t("1");
    // Hashmap_set(hashmap, key, value);
    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(2);
    // value = p2t("2");
    // Hashmap_set(hashmap, key, value);
    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(3);
    // value = p2t("3");
    // Hashmap_set(hashmap, key, value);
    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(11);
    // value = p2t("11");
    // Hashmap_set(hashmap, key, value);
    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(12);
    // value = p2t("12");
    // Hashmap_set(hashmap, key, value);
    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");

    CU_ASSERT(Hashmap_has(hashmap, getTSi(3)));
    //CU_ASSERT(1);
    Hashmap_uninit(hashmap, NULL);
    
}

static void test_hashmap_get (void) 
{
    CU_ASSERT(1);
}

static void test_hashmap_del(void)
{
    CU_ASSERT(1);
}

int do_hashmap_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Hashmap Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test hashmap set", test_hashmap_set) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test hashmap get", test_hashmap_get) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test hashmap del", test_hashmap_del) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}
