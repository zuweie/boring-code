/*
 * @Author: your name
 * @Date: 2020-10-14 21:29:01
 * @LastEditTime: 2020-10-18 11:03:45
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
#include "container/List.h"
#define PRINTF_HASH_NODE(it) \
    ({                                         \
        HashNode* pnode = It_getptr(it);    \
        PRINTF_TV_ON_X(pnode->entity.key, key, %2d, t2i);     \
        PRINTF_TV_ON_X(pnode->entity.value, value, %s, t2p); \
        printf("index: %2d ", pnode->slot_index);             \
        printf("\n");                                       \
    })
#define HASH_KEYS(it)           \
    ({                          \
        Tv key = It_getint(it); \
        int index = hashmap_ikey_hasher(key, HASHMAP_SLOT_SIZE); \
        It_dref(it) = index; \
    }) 

static int  suite_success_init (void) 
{
    printf("\nHashmap suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nHashmap suite success clean\n");
}

static void checkKeysDistribution(Hashmap* hashmap) 
{
    // size_t key_size = CN_size(*hashmap);
    // Tv keys[key_size];
    // Hashmap_keys((*hashmap), keys);

    // List keylist;
    // List_init(keylist, NULL);
    // Arr_to_cn(keys, key_size, t2t, keylist);

    // CN_travel(keylist, HASH_KEYS);


    // List_uninit(keylist,NULL);
}

static void test_hashmap_set (void) 
{
    const int HASHMAP_SIZE = 100;
    Hashmap hashmap;
    Hashmap_init(hashmap, hashmap_ikey_hasher, EQUL);

    Tv key, value;
    Container hashtab;
    for(int i=0; i<HASHMAP_SIZE; ++i) {
        Tv key = getTSi(i);
        Tv value = getTSs(i);
        Hashmap_set(hashmap, key, value);
    }
    
    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");
    
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
    const int HASHMAP_SIZE = 20;
    Hashmap hashmap;
    Hashmap_init(hashmap, hashmap_ikey_hasher, EQUL);

    Tv key, value;
    Container hashtab;
    for(int i=0; i<HASHMAP_SIZE; ++i) {
        Tv key = getTSi(i);
        Tv value = getTSs(i);
        Hashmap_set(hashmap, key, value);
    }
    for (int i=0; i<HASHMAP_SIZE; ++i) {

        CU_ASSERT(Hashmap_has(hashmap, getTSi(i)));
    }

    CU_ASSERT_FALSE(Hashmap_has(hashmap, getTSi(87)));
    CU_ASSERT_FALSE(Hashmap_has(hashmap, getTSi(82)));
    Hashmap_uninit(hashmap, NULL);
}


static void test_hashmap_del(void)
{
    Hashmap hashmap;
    Hashmap_init(hashmap, hashmap_ikey_hasher, EQUL);
    Container hashtab;
    Tv key, value;
    key   = i2t(1);
    value = p2t("1");
    Hashmap_set(hashmap, key, value);
    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");

    key   = i2t(2);
    value = p2t("2");
    Hashmap_set(hashmap, key, value);
    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");

    key   = i2t(3);
    value = p2t("3");
    Hashmap_set(hashmap, key, value);
    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");
    
    key   = i2t(11);
    value = p2t("11");
    Hashmap_set(hashmap, key, value);
    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");

    key   = i2t(12);
    value = p2t("12");
    Hashmap_set(hashmap, key, value);

    key   = i2t(22);
    value = p2t("22");
    Hashmap_set(hashmap, key, value);

    // printf("\n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");

    Hashmap_del(hashmap, i2t(11));
    Hashmap_del(hashmap, i2t(22));
    Hashmap_del(hashmap, i2t(3));
    // printf("\n\n after del \n\n");
    // hashtab = Hashmap_table(hashmap);
    // CN_travel(hashtab, PRINTF_HASH_NODE);
    // printf("\n\n");

    //CU_ASSERT(1);
    CU_ASSERT_FALSE(Hashmap_has(hashmap, i2t(11)));
    CU_ASSERT_FALSE(Hashmap_has(hashmap, i2t(22)));
    CU_ASSERT_FALSE(Hashmap_has(hashmap, i2t(3)));
    
    Hashmap_uninit(hashmap, NULL);
}

static void test_hashmap_set_n_get (void) 
{

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