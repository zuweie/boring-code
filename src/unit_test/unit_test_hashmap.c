/*
 * @Author: your name
 * @Date: 2020-10-14 21:29:01
 * @LastEditTime: 2021-06-29 14:04:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_hashmap.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/Entity.h"
#include "container/Cn.h"
#include "unit_test/unit_test.h"
#include "unit_test/test_data.h"
#include "unit_test/cmp_component.h"
#include "container/List.h"
#include "container/HashMap.h"
#include "container/TreeMap.h"

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

static void test_hashmap_set (void) 
{
    const int HASHMAP_SIZE = 100;
    Map hashmap = _Hashmap(entity_int_keyhasher);

    Tv key, value;

    // Container hashtab;
    for(int i=0; i<HASHMAP_SIZE; ++i) {
        key = getTSi(i);
        value = getTSs(i);
        Map_set(&hashmap, key, value);
    }
    
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");
    
    // key   = i2t(1);
    // value = p2t("1");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_travel(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(2);
    // value = p2t("2");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(3);
    // value = p2t("3");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(11);
    // value = p2t("11");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(12);
    // value = p2t("12");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key = i2t(12);
    // value = p2t("1222");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    //CU_ASSERT_FALSE(Map_has(hashmap, getTSi(3)));

    Tv rdata;
    for (int j=0; j<HASHMAP_SIZE; ++j) {
        int ret = Map_get(&hashmap, getTSi(j), rdata);
        CU_ASSERT_TRUE(ret == 0);
        if (ret == 0) {
            CU_ASSERT_TRUE( CMP_STR(getTSs(j), rdata) == 0);
        }
    }

    Map_set(&hashmap, i2t(12), p2t("1023"));
    Map_get(&hashmap, i2t(12), rdata);
    CU_ASSERT_TRUE( CMP_STR(p2t("1023"), rdata) == 0);

    Map_set(&hashmap, i2t(12), p2t("1024"));
    Map_get(&hashmap, i2t(12), rdata);
    CU_ASSERT_TRUE( CMP_STR(p2t("1024"), rdata) == 0);
    Hashmap_(&hashmap);
    
}

static void test_treemap_set (void) 
{
    const int HASHMAP_SIZE = 100;
    Map treemap = _Treemap(entity_int_insert_cmp);
    Tv key, value;

    // Container hashtab;
    for(int i=0; i<HASHMAP_SIZE; ++i) {
        key = getTSi(i);
        value = getTSs(i);
        Map_set(&treemap, key, value);
    }
    
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");
    
    // key   = i2t(1);
    // value = p2t("1");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_travel(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(2);
    // value = p2t("2");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(3);
    // value = p2t("3");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(11);
    // value = p2t("11");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key   = i2t(12);
    // value = p2t("12");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    // key = i2t(12);
    // value = p2t("1222");
    // Map_set(hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    //CU_ASSERT_FALSE(Map_has(hashmap, getTSi(3)));

    Tv rdata;
    for (int j=0; j<HASHMAP_SIZE; ++j) {

        Tv key = getTSi(j);
        int ret = Map_get(&treemap, key, rdata);
        CU_ASSERT_TRUE(ret == 0);
        if (ret == 0) {
            CU_ASSERT_TRUE( CMP_STR(getTSs(j), rdata) == 0);
        }
    }

    Map_set(&treemap, i2t(12), p2t("1023"));
    Map_get(&treemap, i2t(12), rdata);
    CU_ASSERT_TRUE( CMP_STR(p2t("1023"), rdata) == 0);

    Map_set(&treemap, i2t(12), p2t("1024"));
    Map_get(&treemap, i2t(12), rdata);
    CU_ASSERT_TRUE( CMP_STR(p2t("1024"), rdata) == 0);

    Treemap_(&treemap);
}

static void test_hashmap_get (void) 
{
    const int HASHMAP_SIZE = 20;
    Map hashmap = _Hashmap(entity_int_keyhasher);
    Tv key, value;
    for(int i=0; i<HASHMAP_SIZE; ++i) {
        Tv key = getTSi(i);
        Tv value = getTSs(i);
        Map_set(&hashmap, key, value);
    }
    for (int i=0; i<HASHMAP_SIZE; ++i) {
        CU_ASSERT(Map_has(&hashmap, getTSi(i)));
    }

    CU_ASSERT_FALSE(Map_has(&hashmap, getTSi(87)));
    CU_ASSERT_FALSE(Map_has(&hashmap, getTSi(82)));
    Hashmap_(&hashmap);
}

static void test_treemap_get (void) 
{
    const int HASHMAP_SIZE = 20;
    Map treemap = _Treemap(entity_int_insert_cmp);
    Tv key, value;
    for(int i=0; i<HASHMAP_SIZE; ++i) {
        Tv key = getTSi(i);
        Tv value = getTSs(i);
        Map_set(&treemap, key, value);
    }
    for (int i=0; i<HASHMAP_SIZE; ++i) {
        CU_ASSERT(Map_has(&treemap, getTSi(i)));
    }

    CU_ASSERT_FALSE(Map_has(&treemap, getTSi(87)));
    CU_ASSERT_FALSE(Map_has(&treemap, getTSi(82)));
    Treemap_(&treemap);
}

static void test_hashmap_del(void)
{
    Map hashmap = _Hashmap(entity_int_keyhasher);
    Tv key, value;
    key   = i2t(1);
    value = p2t("1");
    Map_set(&hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    key   = i2t(2);
    value = p2t("2");
    Map_set(&hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    key   = i2t(3);
    value = p2t("3");
    Map_set(&hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");
    
    key   = i2t(11);
    value = p2t("11");
    Map_set(&hashmap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    key   = i2t(12);
    value = p2t("12");
    Map_set(&hashmap, key, value);

    key   = i2t(22);
    value = p2t("22");
    Map_set(&hashmap, key, value);

    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    Map_del(&hashmap, i2t(11), NULL);
    Map_del(&hashmap, i2t(22), NULL);
    Map_del(&hashmap, i2t(3), NULL);
    // printf("\n\n after del \n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    //CU_ASSERT(1);
    CU_ASSERT_FALSE(Map_has(&hashmap, i2t(11)));
    CU_ASSERT_FALSE(Map_has(&hashmap, i2t(22)));
    CU_ASSERT_FALSE(Map_has(&hashmap, i2t(3)));

    Hashmap_(&hashmap);
}

static void test_treemap_del(void)
{
    Map treemap = _Treemap(entity_int_insert_cmp);
    Tv key, value;
    key   = i2t(1);
    value = p2t("1");
    Map_set(&treemap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    key   = i2t(2);
    value = p2t("2");
    Map_set(&treemap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    key   = i2t(3);
    value = p2t("3");
    Map_set(&treemap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");
    
    key   = i2t(11);
    value = p2t("11");
    Map_set(&treemap, key, value);
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    key   = i2t(12);
    value = p2t("12");
    Map_set(&treemap, key, value);

    key   = i2t(22);
    value = p2t("22");
    Map_set(&treemap, key, value);

    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    Map_del(&treemap, i2t(11), NULL);
    Map_del(&treemap, i2t(22), NULL);
    Map_del(&treemap, i2t(3), NULL);
    // printf("\n\n after del \n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");

    //CU_ASSERT(1);
    CU_ASSERT_FALSE(Map_has(&treemap, i2t(11)));
    CU_ASSERT_FALSE(Map_has(&treemap, i2t(22)));
    CU_ASSERT_FALSE(Map_has(&treemap, i2t(3)));

    Treemap_(&treemap);
}

static void test_hashmap_set_n_get (void) 
{
    const int HASHMAP_SIZE = 50;
    
    Tv key, value;
    Map hashmap = _Hashmap(entity_int_keyhasher);
    
    for (int i=0; i<HASHMAP_SIZE; ++i) {
        
        key = getTSi(i);
        value = getTSs(i);
        Map_set(&hashmap, key, value);

    }
    // 随机删到30个数据
    Tv rdata;
    for (int i=0; i<30; ++i) {
        Tv key = getTSi(rand()%50);
        Map_del(&hashmap, key, NULL);
    }

    for (int i=51; i<TEST_DATA_SIZE; ++i) {
        key = getTSi(i);
        value = getTSs(i);
        Map_set(&hashmap, key, value);
    }

    List list = _List(EQUL);
    //Container table = Hashmap_table(hashmap);
    CN_duplicate(&hashmap, &list);

    // printf("\n\n");
    CN_foreach(&list, ENTITY_2_HASH_KEY);
    // printf("\n\n indexs: ");
    // CN_foreach(list, PRINTF_TV_ON_INT);
    // printf("\n\n");
    // printf("size of index: %d\n", CN_size(list));

    CN_wring(&list, NULL);
    // printf("\n after wring :\n");
    // CN_foreach(list, PRINTF_TV_ON_INT);

    CN_sort(&list, CMP_INT);
    
    // printf("\n after sort :\n");
    // CN_foreach(list, PRINTF_TV_ON_INT);

    for(It first = CN_first(&list); !It_equal(first, CN_last(&list)); first=It_next(first)){

        It next = It_next(first);
        int v1 = It_getint(first);
        int v2 = It_getint(next);

        CU_ASSERT_FALSE( v1 == v2);
    }

    List_(&list, NULL);
    Hashmap_(&hashmap);
}

static void test_treemap_first_last(void) 
{
    Map treemap = _Treemap(entity_int_insert_cmp);
    Map_set(&treemap, 1, 1);
    It first = CN_first(&treemap);
    Entity* entity = It_getptr(first);
    CU_ASSERT_TRUE( CMP_INT(entity->tv[0], i2t(1)) == 0 );

    It last = CN_last(&treemap);
    entity = It_getptr(last);
    CU_ASSERT_TRUE( CMP_INT(entity->tv[0], i2t(1)) == 0 );

    // 插入一个2
    Map_set(&treemap, 2, 2);

    first = CN_first(&treemap);
    entity = It_getptr(first);
    CU_ASSERT_TRUE( CMP_INT(entity->tv[0], i2t(1)) == 0);

    last = CN_last(&treemap);
    entity = It_getptr(last);
    CU_ASSERT_TRUE( CMP_INT(entity->tv[0], i2t(2)) == 0);
    //PRINTF_TV_ON_INT(entity->tv[0]);
    
    Map_del(&treemap, 1, NULL);
    first = CN_first(&treemap);
    entity = It_getptr(first);
    CU_ASSERT_TRUE( CMP_INT(entity->tv[0], i2t(2)) == 0);


    last  = CN_last(&treemap);
    entity = It_getptr(last);
    CU_ASSERT_TRUE( CMP_INT(entity->tv[0], i2t(2)) == 0);
    Treemap_(&treemap);
    
    Map treemap2 = _Treemap(entity_int_insert_cmp);
    List list    = _List(NULL);
    for (int i=0; i<100; ++i) {
        Map_set(&treemap2, getTSi(i), getTSi(i));
        CN_add(&list, getTSi(i));
    }
    CN_sort(&list, CMP_INT);
    
    Tv list_first = It_dref( CN_first(&list) );
    Tv list_last  = It_dref( CN_last(&list) );
    Tv list_sec   = It_dref( It_next(CN_first(&list)) );
    Tv list_rsec  = It_dref( It_prev(CN_last(&list)) );

    It tree2_first = CN_first(&treemap2);
    Entity* first_entity = It_getptr(tree2_first);
    It tree2_last  = CN_last(&treemap2);
    Entity* last_entity = It_getptr(tree2_last);
    CU_ASSERT_TRUE(CMP_INT(first_entity->tv[0], list_first) == 0);
    CU_ASSERT_TRUE(CMP_INT(last_entity->tv[0], list_last) == 0);

    Map_del(&treemap2, getTSi(45), NULL);
    Map_del(&treemap2, getTSi(53), NULL);

    tree2_first = CN_first(&treemap2);
    first_entity = It_getptr(tree2_first);
    tree2_last  = CN_last(&treemap2);
    last_entity = It_getptr(tree2_last);

    CU_ASSERT_TRUE(CMP_INT(first_entity->tv[0], list_first) == 0);
    CU_ASSERT_TRUE(CMP_INT(last_entity->tv[0], list_last) == 0);
    
    // printf("\n");
    // for (It first = CN_first(treemap2); !It_equal(first, CN_tail(treemap2)); first = It_next(first)) 
    // {
    //     Entity* entity = It_getptr(first);
    //     PRINTF_TV_ON_INT(entity->tv[0]);
    // }
    // printf("\n");

    Map_del(&treemap2, list_first, NULL);
    Map_del(&treemap2, list_last, NULL);

    // printf("\n");
    // for (It first = CN_first(treemap2); !It_equal(first, CN_tail(treemap2)); first = It_next(first)) 
    // {
    //     Entity* entity = It_getptr(first);
    //     PRINTF_TV_ON_INT(entity->tv[0]);
    // }
    // printf("\n");

    tree2_first = CN_first(&treemap2);
    first_entity = It_getptr(tree2_first);
    tree2_last  = CN_last(&treemap2);
    last_entity = It_getptr(tree2_last);

    
    CU_ASSERT_FALSE(CMP_INT(first_entity->tv[0], list_first) == 0);
    CU_ASSERT_FALSE(CMP_INT(last_entity->tv[0], list_last) == 0);

    CU_ASSERT_TRUE(CMP_INT(first_entity->tv[0], list_sec) == 0);
    CU_ASSERT_TRUE(CMP_INT(last_entity->tv[0], list_rsec) == 0);

    Treemap_(&treemap2);
    List_(&list, NULL);
}

static void test_entity(void)
{
    Entity entity;
    Tv tv[4];
    Entity_temp(&entity, 4, 2, tv, i2t(12), i2t(23), f2t(44.55694), f2t(2839.783874));
    //TempEntity(&entity, 4, 2, tv, 12, 23, 44.55694, 2839.783874);
    // PRINTF_TV_ON_INT(tv[0]);
    // PRINTF_TV_ON_INT(tv[1]);
    // PRINTF_TV_ON_FLOAT(tv[2]);
    // PRINTF_TV_ON_FLOAT(tv[3]);
    CU_ASSERT_TRUE(t2i(tv[0]) == 12);
    CU_ASSERT_TRUE(t2i(tv[1]) == 23);
    CU_ASSERT_TRUE(t2f(tv[2]) == 44.55694);
    CU_ASSERT_TRUE(t2f(tv[3]) == 2839.783874);
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

    if (NULL == CU_add_test(pSuite, "test hashmap set and get", test_hashmap_set_n_get) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test treemap set", test_treemap_set) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test treemap get", test_treemap_get) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test treemap del", test_treemap_del) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test entity", test_entity) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test treemap first last", test_treemap_first_last) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}