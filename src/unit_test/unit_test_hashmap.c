/*
 * @Author: your name
 * @Date: 2020-10-14 21:29:01
 * @LastEditTime: 2021-10-27 10:19:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_hashmap.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/entity.h"
#include "container/cn.h"
#include "unit_test/unit_test.h"
#include "unit_test/test_data.h"
// #include "unit_test/cmp_component.h"
// #include "container/List.h"
// #include "container/HashMap.h"
// #include "container/TreeMap.h"

#define PRINTF_ENTITY(e, keyx, key_trans, valuex, value_trans) \
    ({                                                         \
        printf("keys: ");                                      \
        for(int i=0; i<e->value_index; ++i) {                  \
            char label_str[10];                                \
            sprintf(label_str, "key<%d>", i);                  \
            PRINTF_TV_ON_X(e->tv[i], label_str, keyx, key_trans); \
        } \
        /*printf("\n"); */\
        printf("values: "); \
        for(int i=e->value_index; i<e->number; ++i) { \
            char label_str[10];                       \
            sprintf(label_str, "val<%d>", i);         \
            PRINTF_TV_ON_X(e->tv[i], label_str, valuex, value_trans); \
        } \
        printf("\n"); \
    })
#define PRINTF_HASH_NODE(tv) \
    ({                                \
        HashmapNode* pnode = t2p(tv);       \
        Entity* pentity = t2p(pnode->entity);               \
        PRINTF_ENTITY( pentity, %d, t2i, %s, t2p);          \
        printf("index: %2d ", pnode->slot_index);           \
        printf("\n\n");                                       \
    })
#define ENTITY_2_HASH_KEY(tv) do {     \
    HashmapNode * pnode  =  t2p(tv);   \
    int index = entity_int_keyhasher(pnode->entity, HASHMAP_SLOT_SIZE); \
    tv = i2t(index); \
} while(0)

// static int ikey_hasher (Tv v1, size_t slot_size) 
// {
//     Entity* entity = t2p(v1);
//     return t2i(entity->tv[0]) % slot_size; 
// }

// static int key_insert_cmp(Tv v1, Tv v2) 
// {
//     Entity* e1 = t2p(v1);
//     Entity* e2 = t2p(v2);
//     if (e1->value_index == e2->value_index) {
//         for (int i=0; i<e1->value_index; ++i) {
            
//             // 直接用 int 的方式来比较大小。
//             int ret = Tv_cmpi(e1->tv[i], e2->tv[i]);
//             if (ret !=0 )return ret;
//         }
//         // 相等。
//         return 0;
//     } else {
//         return e1->value_index > e2->value_index ? 1: -1;
//     }
// }

static int  suite_success_init (void) 
{
    printf("\nHashmap suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nHashmap suite success clean\n");
}

static void test_hashmap (void) 
{
    const int HASHMAP_SIZE = 100;
    //CN hashmap = _Hashmap(entity_int_keyhasher);
    CN hashmap = CN_create(HASH_MAP, int_t, str_t);

    int key;
    char* value;

    // Container hashtab;
    for(int i=0; i<HASHMAP_SIZE; ++i) {
        key = tsd_get_int(i);
        value = tsd_get_str(i);
        CN_set(hashmap, key, value);
    }

    //Tv rdata;
    for (int j=0; j<HASHMAP_SIZE; ++j) {
        char** ret = CN_get(hashmap, tsd_get_int(j));
        CU_ASSERT_TRUE(ret != NULL);
        CU_ASSERT_STRING_EQUAL(*ret, tsd_get_str(j));
    }

    CN_set(hashmap, 12, "1023");
    char** rdata = CN_get(hashmap, 12);
    CU_ASSERT_STRING_EQUAL(*rdata, "1023");

    CN_set(hashmap, 12, "1024");
    rdata = CN_get(hashmap, 12);
    CU_ASSERT_STRING_EQUAL(*rdata, "1024");
    CN_finalize(hashmap, NULL);
    
}

static void test_treemap_set (void) 
{
    const int HASHMAP_SIZE = 100;
    CN treemap = CN_create(TREE_MAP, int_t, str_t);
    int key;
    char* value;

    // Container hashtab;
    for(int i=0; i<HASHMAP_SIZE; ++i) {
        key = tsd_get_int(i);
        value = tsd_get_str(i);
        CN_set(treemap, key, value);
    }

    char** rdata;
    for (int j=0; j<HASHMAP_SIZE; ++j) {

        rdata = CN_get(treemap, tsd_get_int(j));
        CU_ASSERT_TRUE(*rdata != 0);
        CU_ASSERT_STRING_EQUAL(*rdata, tsd_get_str(j));
    }

    CN_set(treemap, 12, "1023");
    rdata = CN_get(treemap, 12);
    CU_ASSERT_STRING_EQUAL(*rdata, "1023");

    CN_set(treemap, 12, "1024");
    rdata = CN_get(treemap, 12);
    CU_ASSERT_STRING_EQUAL(*rdata, "1024");

    CN_finalize(treemap, NULL);
}

static void test_hashmap_del(void)
{
    CN hashmap = CN_create(HASH_MAP, int_t, str_t);
    CN_set(hashmap, 1, "1");
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    CN_set(hashmap, 2, "2");
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    CN_set(hashmap, 3, "3");
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");
    

    CN_set(hashmap, 11, "11");
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");
    CN_set(hashmap, 12, "12");


    CN_set(hashmap, 22, "22");

    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    CN_del(hashmap, 11);
    CN_del(hashmap, 22);
    CN_del(hashmap, 3);
    // printf("\n\n after del \n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    //CU_ASSERT(1);
    CU_ASSERT_FALSE(CN_has(hashmap, 11));
    CU_ASSERT_FALSE(CN_has(hashmap, 22));
    CU_ASSERT_FALSE(CN_has(hashmap, 3));

    CU_ASSERT_TRUE(CN_has(hashmap, 12));
    CU_ASSERT_TRUE(CN_has(hashmap, 1));
    CU_ASSERT_TRUE(CN_has(hashmap, 2));
    CU_ASSERT_STRING_EQUAL(T_str(CN_get(1)), "1");

    CN_finalize(hashmap, NULL);
}

static void test_treemap_del(void)
{
    CN treemap = CN_create(TREE_MAP, int_t, str_t);

    CN_set(treemap, 1, "1");
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    CN_set(treemap, 2, "2");
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    CN_set(treemap, 3, "3");
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");
    
    CN_set(treemap, 11, "11");
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");
    CN_set(treemap, 12, "12");

    CN_set(treemap, 22, "22");

    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    CN_del(treemap, 11);
    CN_del(treemap, 22);
    CN_del(treemap, 3);
    // printf("\n\n after del \n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    //CU_ASSERT(1);
    CU_ASSERT_FALSE(CN_has(treemap, 11));
    CU_ASSERT_FALSE(CN_has(treemap, 22));
    CU_ASSERT_FALSE(CN_has(treemap, 33));

    CN_finalize(treemap, NULL);
}


static void test_treemap_first_last(void) 
{
    CN treemap = CN_create(TREE_MAP, int_t, int_t);
    CN_set(treemap, 1, 1);

    It first = CN_first(treemap);
    entity_t* ent = It_ptr(first);
    CU_ASSERT_TRUE(ef_int(ent, 0) == 1);
    CU_ASSERT_TRUE(ef_int(ent, 1) == 1);

    It last = CN_last(treemap);
    ent = It_ptr(last);
    CU_ASSERT_TRUE( ef_int(ent, 0) == 1 );
    CU_ASSERT_TRUE( ef_int(ent, 1) == 1 );

    // 插入一个2
    CN_set(treemap, 2, 2);

    first = CN_first(treemap);
    ent = It_ptr(first);
    CU_ASSERT_TRUE( ef_int(ent, 0) == 1);
    CU_ASSERT_TRUE( ef_int(ent, 1) == 1);

    last = CN_last(treemap);
    ent = It_ptr(last);
    CU_ASSERT_TRUE( ef_int(ent, 0) == 2);
    CU_ASSERT_TRUE( ef_int(ent, 1) == 2 );
    
    CN_del(treemap, 1);
    first = CN_first(treemap);
    ent = It_ptr(first);
    CU_ASSERT_TRUE( ef_int(ent, 0) == 2);
    CU_ASSERT_TRUE( ef_int(ent, 1) == 2 );


    last  = CN_last(treemap);
    ent = It_ptr(last);
    CU_ASSERT_TRUE( ef_int(ent, 0) == 2);
    CN_finalize(treemap, NULL);
// -----------------------------------------------------------

    CN treemap2 = CN_create(TREE_MAP, int_t, int_t);
    CN list     = CN_create(LIST, int_t);

    for (int i=0; i<100; ++i) {
        int v1 = tsd_get_int(i);
        int v2 = tsd_get_int(i);

        CN_set(treemap2,v1, v2);
        CN_add(list, v1);
    }

    CN_sort(list, NULL);
    
    It ls_first = CN_first(list);
    It ls_last  = CN_last(list);

    int list_first = It_int( ls_first );
    int list_last  = It_int( ls_last );

    It_next(ls_first);
    It_prev(ls_last);
    int list_sec   = It_int( ls_first );
    int list_rsec  = It_int( ls_last );

    It tree2_first = CN_first(treemap2);
    entity_t* first_entity = It_ptr(tree2_first);

    It tree2_last  = CN_last(treemap2);
    entity_t* last_entity = It_ptr(tree2_last);

    CU_ASSERT_TRUE(  ef_int(first_entity, 0) == list_first );/*CMP_INT(first_entity->tv[0], list_first) == 0*/
    CU_ASSERT_TRUE( ef_int(last_entity, 0) == list_last );/*CMP_INT(last_entity->tv[0], list_last) == 0*/

    CN_del(treemap2, 45);
    CN_del(treemap2, 53);

    tree2_first = CN_first(treemap2);
    first_entity = It_ptr(tree2_first);

    tree2_last  = CN_last(treemap2);
    last_entity = It_ptr(tree2_last);

    CU_ASSERT_TRUE(ef_int(first_entity, 0) == list_first);
    CU_ASSERT_TRUE(ef_int(last_entity, 0) ==list_last);
    
    // printf("\n");
    // for (It first = CN_first(treemap2); !It_equal(first, CN_tail(treemap2)); first = It_next(first)) 
    // {
    //     Entity* entity = It_getptr(first);
    //     PRINTF_TV_ON_INT(entity->tv[0]);
    // }
    // printf("\n");

    CN_del(treemap2, list_first);
    CN_del(treemap2, list_last);

    // printf("\n");
    // for (It first = CN_first(treemap2); !It_equal(first, CN_tail(treemap2)); first = It_next(first)) 
    // {
    //     Entity* entity = It_getptr(first);
    //     PRINTF_TV_ON_INT(entity->tv[0]);
    // }
    // printf("\n");

    tree2_first = CN_first(treemap2);
    first_entity = It_ptr(tree2_first);

    tree2_last  = CN_last(treemap2);
    last_entity = It_ptr(tree2_last);

    
    CU_ASSERT_FALSE(ef_int(first_entity, 0) ==  list_first);
    CU_ASSERT_FALSE(ef_int(last_entity, 0) == list_last);

    CU_ASSERT_TRUE(ef_int(first_entity, 0) == list_sec);
    CU_ASSERT_TRUE(ef_int(last_entity, 0) == list_rsec);

    CN_finalize(treemap2, NULL);
    CN_finalize(list, NULL);
}



int do_hashmap_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Hashmap Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test hashmap set", test_hashmap) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }



    if (NULL == CU_add_test(pSuite, "test hashmap del", test_hashmap_del) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    
    if (NULL == CU_add_test(pSuite, "test treemap set", test_treemap_set) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    if (NULL == CU_add_test(pSuite, "test treemap del", test_treemap_del) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    

    if (NULL == CU_add_test(pSuite, "test treemap first last", test_treemap_first_last) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}