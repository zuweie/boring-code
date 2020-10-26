/*
 * @Author: zuweie
 * @Date: 2020-09-22 15:01:45
 * @LastEditTime: 2020-10-26 08:23:01
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
#define cc(con)              (((Container*)( &(con) ))->_container)
#define c_search_cmp(con)    (((Container*)( &(con) ))->_search_compare)
#define c_conflict_fix(con)  (((Container*)( &(con) ))->_conflict_fix)
#define c_setup(con)         (((Container*)( &(con) ))->_setup)
#define c_sort_cmp(con)      (((Container*)( &(con) ))->_sort_compare)
#define c_wring_cmp(con)     (((Container*)( &(con) ))->_wring_compare)
#define c_wring_cb(con)      (((Container*)( &(con) ))->_wring_callback)
#define c_cleanup(con)       (((Container*)( &(con) ))->_cleanup_handler)

#define set_search_cmp(con, search_cmp)     (c_search_cmp(con)=search_cmp)
#define set_setup(con, setup)               (c_setup(con) = setup)
#define set_conflict_fix(con, conflict_fix) (c_conflict_fix(con)=conflict_fix)
#define set_sort_cmp(con, sort_cmp)         (c_sort_cmp(con)=sort_cmp)
#define set_wring_cmp(con, wring_cmp)       (c_wring_cmp(con)=wring_cmp)
#define set_wring_cb(con, wring_cb)         (c_wring_cb(con)=wring_cb)
#define set_cleanup(con, cleanup)           (c_cleanup(con)=cleanup)


#define CN_first(con) container_first(cc(con))
#define CN_last(con) container_last(cc(con))
#define CN_head(con) container_head(cc(con))
#define CN_tail(con) container_tail(cc(con))

#define CN_search(con,offset,find) container_search(cc(con), offset, find, c_search_cmp(con))
#define CN_find(con,find) CN_search(con, CN_first(con), find)

// 特殊的插入。
#define CN_set(con, data) container_set(cc(con), data, c_setup(con),  c_conflict_fix(con))

#define CN_insert(con, it, data) container_insert(cc(con), it, data)
// 头部插入
#define CN_add_first(con, data) CN_insert(con, CN_first(con), data)
// 尾部插入
#define CN_add_tail(con, data) CN_insert(con, CN_tail(con), data)

#define CN_add(con, data) CN_add_tail(con, data)

#define CN_remove(con, it, rdata) container_remove(cc(con), it, rdata)
// 头部移除
#define CN_rm_first(con, rdata) CN_remove(con, CN_first(con), rdata)
// 尾部移除
#define CN_rm_last(con, rdata) CN_remove(con, CN_last(con), rdata)

#define CN_rm(con, rdata) CN_rm_last(con, rdata)
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
#define CN_sort(con) container_sort(cc(con), c_sort_cmp(con))
#define CN_wring(con, cb) container_wring(cc(con), c_wring_cmp(con), c_wring_cb(con))

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
            handle(It_dref(first));                 \
        }                                           \
}while(0)

#define CN_foreach(con, handle) CN_travel(con, handle)

#define CN_unique(con) do {    \
    CN_sort(con, csc(con));    \                  
    CN_wring(con, cwb(con));   \            
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

#define CN_initialize(con, label, search_cmp, setup,conflict_fix, sort_cmp, wring_cmp, wring_cb, cleanup, ... ) do {  \
    set_search_cmp(con, search_cmp);      \
    set_conflict_fix(con, conflict_fix);  \
    set_setup(con, setup);                \
    set_sort_cmp(con, sort_cmp);          \
    set_wring_cmp(con, wring_cmp);        \
    set_wring_cb(con, wring_cb);          \
    set_cleanup(con, cleanup);            \
    cc(con) = container_create(label, __VA_ARGS__); \
}while(0)

#define CN_uninitialize(con, label) do {    \
    if (c_cleanup(con)) {                   \
        CN_travel(con, c_cleanup(con));     \
    }                                       \
    container_destroy(label, cc(con));      \
    cc(con) = NULL;                         \
}while(0)

#define _CN(pcon)                 \
    ({                            \
        Container con = {         \
            ._container = (pcon), \
        };                        \
        con;                      \
    })

typedef int (*Cleaner)(It);
typedef struct _con{
    
    container_t* _container;

    int (*_search_compare)(Tv, Tv);
    int (*_setup)(Tv*, Tv);
    int (*_conflict_fix)(Tv, Tv);
    int (*_sort_compare)(Tv, Tv);
    int (*_wring_compare)(Tv, Tv);
    int (*_wring_callback)(Tv, void*);
    int (*_cleanup_handler) (Tv);

} Container;

// 这个能比较整数和浮点的值是否相等，但不能比较大小，这个只是用 ^ 做位运算，只能得出是否相等，不知其大小。
static inline 
int EQUL (Tv v1, Tv v2) {
    return Tv_equl(v1, v2);
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