/*
 * @Author: zuweie
 * @Date: 2020-09-22 15:01:45
<<<<<<< HEAD
 * @LastEditTime: 2020-10-23 12:58:05
=======
 * @LastEditTime: 2020-10-23 01:17:42
>>>>>>> 9def592acf81ccf931381a808989fbfb2bf43559
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/cn.h
 */
#ifndef _CON_H_
#define _CON_H_
#include <string.h>
#include "base/__container.h"
#include "It.h"
#include "Tv.h"

/* container function */
#define cc(con)   (((Container*)( &(con) ))->_container)
#define ccmp(con) (((Container*)( &(con) ))->_compare)

//#define cc(con) ((con)._container)
//#define ccmp(con) ((con)._compare)

#define CN_first(con) container_first(cc(con))
#define CN_last(con) container_last(cc(con))
#define CN_head(con) container_head(cc(con))
#define CN_tail(con) container_tail(cc(con))

#define CN_search(con,offset,find) container_search(cc(con), offset, find, ccmp(con))
#define CN_find(con,find) CN_search(con, CN_first(con), find)

// 特殊的插入。
#define CN_set(con, data, setup) container_set(cc(con), data, setup)

#define CN_insert(con, it, data) container_insert(cc(con), it, data)
// 头部插入
#define CN_add_first(con, data) CN_insert(con, CN_first(con), data)
// 尾部插入
#define CN_add_tail(con, data) CN_insert(con, CN_tail(con), data)

#define CN_add(con, data) CN_add_tail(con, data)

#define CN_remove(con, it) container_remove(cc(con), it)
// 头部移除
#define CN_rm_first(con) CN_remove(con, CN_first(con))
// 尾部移除
#define CN_rm_last(con) CN_remove(con, CN_last(con))

#define CN_rm(con) CN_rm_last(con)
// 移除特定目标
#define CN_rm_target(con, find, ret)   \
    ({                                 \
        int ret_code = -1;             \
        It pos = CN_find(con, find);   \
        if (It_valid(pos)) {           \
            ret_code = CN_remove(con, pos, ret); \
        }                                        \
        ret_code;                                \
    })

//#define chas(con, find) container_has(cc(con), find, ccmp(con))
#define CN_size(con) container_size(cc(con))
#define CN_sort(con, cmp) container_sort(cc(con), cmp)
#define CN_wring(con, cb) container_wring(cc(con), ccmp(con), cb)

#define CN_has(con, find)            \
    ({                               \
        It pos = CN_find(con, find); \
        int ret = It_valid(pos);     \
        ret;                         \
    })

#define CN_to_arr(con, arr) do { \
    int i = 0;                    \
    for (It first = CN_first(con);      \
        !It_equal(first, CN_tail(con)); \
        first = It_next(first) ) {      \
            arr[i++] = It_dref(first);  \
        } \
}while(0)

#define Arr_to_cn(arr, size, transfer, con) do { \
    for (int i=0; i<size; ++i) {              \
        Tv v = transfer(arr[i]);              \
        CN_add_tail(con, v);                  \
    }                                         \
}while(0)
// 遍历容器，
#define CN_travel(con, handle) do {                 \
    for(It first = CN_first(con);                   \
        !It_equal(first, CN_tail(con));             \
        first = It_next(first) ) {                  \
            handle(first);                          \
        }                                           \
}while(0)

#define CN_foreach(con, handle) CN_travel(con, handle)

#define CN_unique(con, sort_cmp) do {    \
    CN_sort(con, sort_cmp);              \                  
    CN_wring(con, NULL);                 \            
}while(0)

#define CN_duplicate(con1, con2) do {    \
    for(It first = CN_first(con1);       \
        !It_equal(first, CN_tail(con1)); \
        first=It_next(first)) {          \
        CN_add(con2, It_dref(first));    \
    }                                    \ 
} while(0)


// 两个容器合并。
#define CN_merge(con1, con2) do { \
    It first2  = CN_first(c2);    \
    It tail2   = CN_tail(c2);     \
    for(; !It_equal(first2, tail2); \
        first2 = It_next(first2)) { \
        Tv data = It_dref(first2);    \
        CN_add(c1, data);             \
    }                                 \
}while(0)

#define CN_initialize(con, label, cmp, ... ) do {     \
    cc(con) = container_create(label, __VA_ARGS__);   \
    if (cmp) {                                        \
        ccmp(con) = cmp;                              \
    }else{                                            \
        ccmp(con) = EQUL;                             \
    }                                                 \
}while(0)

#define CN_uninitialize(con, label, cleanup) do { \
    Cleaner cleaner = (Cleaner)cleanup;           \
    if (cleaner) {                                \
        CN_travel(con, cleaner);                  \
    }                                             \
    container_destroy(label, cc(con));            \
    ccmp(con) = NULL;                             \
    cc(con) = NULL;                               \
}while(0)

#define _CN(pcon, cmp)            \
    ({                            \
        Container con = {         \
            ._container = (pcon), \
            ._compare = (cmp)     \
        };                        \
        con;                      \
    })

typedef int (*Cleaner)(It);
typedef struct _con{
    
    container_t* _container;
    int (*_compare)(type_value_t, type_value_t);

} Container;

// 这个能比较整数和浮点的值是否相等，但不能比较大小，这个只是用 ^ 做位运算，只能得出是否相等，不知其大小。
static inline 
int EQUL (Tv v1, Tv v2) {
    v_type result = Tv_equl(v1, v2);
    return result?1:0;
}
// 这个先用位运算比较是否相等，不等的话再化做 int 形式比较大小
static inline
int CMP_INT (Tv v1, Tv v2) 
{
    return Tv_cmpi(v1, v2);
}
static inline 
int INCMP_INT (Tv v1, Tv v2) 
{
    return Tv_cmpi(v2, v1);
}

// 这个先用位运算比较是否相等，不等的话再化做 float 形式比较大小
static inline
int CMP_FLT (Tv v1, Tv v2) 
{
    return Tv_cmpf(v1, v2);
}
static inline
int INCMP_FLT (Tv v1, Tv v2) 
{
    return Tv_cmpf(v2, v1);
}

// 指针比较
static inline
int CMP_PTR (Tv v1, Tv v2) 
{
    return Tv_cmpptr(v1, v2);
}

// 比较字符串
static inline
int CMP_STR (Tv v1, Tv v2) 
{
    const char* s1 = t2p(v1);
    const char* s2 = t2p(v2);
    int ret = strcmp(s1, s2);
    return ret?(ret>0?1:-1):0;
}

static inline
int INCMP_STR(Tv v1, Tv v2) 
{
    return CMP_STR(v2, v1);
}
#endif