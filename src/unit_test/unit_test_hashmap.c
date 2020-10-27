/*
 * @Author: your name
 * @Date: 2020-10-14 21:29:01
 * @LastEditTime: 2020-10-27 23:20:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_hashmap.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/Hashmap.h"
#include "container/Entity.h"
#include "container/Cn.h"
#include "unit_test/unit_test.h"
#include "unit_test/test_data.h"
#include "container/List.h"
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
        HashNode* pnode = t2p(tv);/*It_getptr(it);*/        \
        Entity* pentity = t2p(pnode->entity);               \
        PRINTF_ENTITY( pentity, %d, t2i, %s, t2p);          \
        printf("index: %2d ", pnode->slot_index);           \
        printf("\n\n");                                       \
    })
#define ENTITY_2_HASH_KEY(tv) do {       \
    HashNode * pnode  =  t2p(tv); /*It_getptr(it);*/  \
    int index = ikey_hasher(pnode->entity, HASHMAP_SLOT_SIZE); \
    /*It_dref(it)*/ tv = i2t(index);  \
} while(0)

static int ikey_hasher (Tv v1, size_t slot_size) 
{
    Entity* entity = t2p(v1);
    return t2i(entity->tv[0]) % slot_size; 
}

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
    Hashmap hashmap = _Hashmap(ikey_hasher, Entity_key_cmp, Hashmap_setup, Hashmap_conflict_fix);

    Tv key, value;

    // Container hashtab;
    // for(int i=0; i<HASHMAP_SIZE; ++i) {
    //     Hashmap_set(hashmap, key, value);
    // }
    
    // printf("\n\n");
    // CN_foreach(hashmap, PRINTF_HASH_NODE);
    // printf("\n\n");
    
    key   = i2t(1);
    value = p2t("1");
    Hashmap_set(hashmap, key, value);
    printf("\n\n");
    CN_travel(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");

    key   = i2t(2);
    value = p2t("2");
    Hashmap_set(hashmap, key, value);
    printf("\n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");

    key   = i2t(3);
    value = p2t("3");
    Hashmap_set(hashmap, key, value);
    printf("\n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");

    key   = i2t(11);
    value = p2t("11");
    Hashmap_set(hashmap, key, value);
    printf("\n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");

    key   = i2t(12);
    value = p2t("12");
    Hashmap_set(hashmap, key, value);
    printf("\n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");

    printf("hihihihihihi\n");
    key = i2t(12);
    value = p2t("1222");
    Hashmap_set(hashmap, key, value);
    printf("\n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");
    printf("hihihihihihi\n");
    
    CU_ASSERT(Hashmap_has(hashmap, getTSi(3)));
    Hashmap_(hashmap, Hashmap_cleanup_entity);
    
}

static void test_hashmap_get (void) 
{
    const int HASHMAP_SIZE = 20;
    Hashmap hashmap = _Hashmap(ikey_hasher, Entity_key_cmp, Hashmap_setup, Hashmap_conflict_fix);

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
    Hashmap_(hashmap, Hashmap_cleanup_entity);
}


static void test_hashmap_del(void)
{
    Hashmap hashmap = _Hashmap(ikey_hasher, Entity_key_cmp, Hashmap_setup, Hashmap_conflict_fix);
    Tv key, value;
    key   = i2t(1);
    value = p2t("1");
    Hashmap_set(hashmap, key, value);
    printf("\n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");

    key   = i2t(2);
    value = p2t("2");
    Hashmap_set(hashmap, key, value);
    printf("\n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");

    key   = i2t(3);
    value = p2t("3");
    Hashmap_set(hashmap, key, value);
    printf("\n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");
    
    key   = i2t(11);
    value = p2t("11");
    Hashmap_set(hashmap, key, value);
    printf("\n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");

    key   = i2t(12);
    value = p2t("12");
    Hashmap_set(hashmap, key, value);

    key   = i2t(22);
    value = p2t("22");
    Hashmap_set(hashmap, key, value);

    printf("\n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");

    Hashmap_del(hashmap, i2t(11));
    Hashmap_del(hashmap, i2t(22));
    Hashmap_del(hashmap, i2t(3));
    printf("\n\n after del \n\n");
    CN_foreach(hashmap, PRINTF_HASH_NODE);
    printf("\n\n");

    //CU_ASSERT(1);
    CU_ASSERT_FALSE(Hashmap_has(hashmap, i2t(11)));
    CU_ASSERT_FALSE(Hashmap_has(hashmap, i2t(22)));
    CU_ASSERT_FALSE(Hashmap_has(hashmap, i2t(3)));

    Hashmap_(hashmap, Hashmap_cleanup_entity);
}

static void test_hashmap_set_n_get (void) 
{
    const int HASHMAP_SIZE = 50;
    
    Tv key, value;
    Hashmap hashmap = _Hashmap(ikey_hasher, Entity_key_cmp, Hashmap_setup, Hashmap_conflict_fix);
    
    for (int i=0; i<HASHMAP_SIZE; ++i) {
        
        key = getTSi(i);
        value = getTSs(i);
        Hashmap_set(hashmap, key, value);

    }
    // 随机删到30个数据
    for (int i=0; i<30; ++i) {
        Tv key = getTSi(rand()%50);
        Hashmap_del(hashmap, key);
    }

    for (int i=51; i<TEST_DATA_SIZE; ++i) {
        key = getTSi(i);
        value = getTSs(i);
        Hashmap_set(hashmap, key, value);
    }

    List list = _List(EQUL);
    //Container table = Hashmap_table(hashmap);
    CN_duplicate(hashmap, list);

    printf("\n\n");
    CN_foreach(list, ENTITY_2_HASH_KEY);
    printf("\n\n indexs: ");
    CN_foreach(list, PRINTF_TV_ON_INT);
    printf("\n\n");
    printf("size of index: %d\n", CN_size(list));

    CN_wring(list, NULL);
    printf("\n after wring :\n");
    CN_foreach(list, PRINTF_TV_ON_INT);

    CN_sort(list, CMP_INT);
    
    printf("\n after sort :\n");
    CN_foreach(list, PRINTF_TV_ON_INT);

    for(It first = CN_first(list); !It_equal(first, CN_last(list)); first=It_next(first)){

        It next = It_next(first);
        int v1 = It_getint(first);
        int v2 = It_getint(next);

        CU_ASSERT_FALSE( v1 == v2);
    }

    List_(list, NULL);
    Hashmap_(hashmap, Hashmap_cleanup_entity);
}

static void test_entity(void)
{
    Entity entity;
    Tv tv[4];
    TempEntity(&entity, 4, 2, tv, i2t(12), i2t(23), f2t(44.55694), f2t(2839.783874));
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
    
    if (NULL == CU_add_test(pSuite, "test entity", test_entity) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}