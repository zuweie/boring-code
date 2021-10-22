/*
 * @Author: zuweie
 * @Date: 2020-09-22 15:01:45
 * @LastEditTime: 2021-06-29 15:31:47
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
#define c_base(con_ptr)          (((Container*)(con_ptr))->_container)
#define c_search_cmp(con_ptr)    (((Container*)(con_ptr))->_search_compare)
#define c_conflict_fix(con_ptr)  (((Container*)(con_ptr))->_conflict_fix)
#define c_setup(con_ptr)         (((Container*)(con_ptr))->_setup)
#define c_extra_func(con_ptr)    (((Container*)(con_ptr))->_extra_func)

#define CN_set_search_cmp(con_ptr, search_cmp)     (c_search_cmp(con_ptr)=search_cmp)
#define CN_set_setup(con_ptr, setup)               (c_setup(con_ptr) = setup)
#define CN_set_conflict_fix(con_ptr, conflict_fix) (c_conflict_fix(con_ptr)=conflict_fix)
#define CN_set_extra_func(con_ptr, func)           (c_extra_func(con_ptr)=func)

#define CN_first(con_ptr) container_first(c_base(con_ptr))
#define CN_last(con_ptr) container_last(c_base(con_ptr))
#define CN_head(con_ptr) container_head(c_base(con_ptr))
#define CN_tail(con_ptr) container_tail(c_base(con_ptr))

#define CN_search(con_ptr,offset,find) container_search(c_base(con_ptr), offset, find, c_search_cmp(con_ptr))
#define CN_find(con_ptr,find) CN_search(con_ptr, CN_first(con_ptr), find)

// 特殊的插入。
#define CN_set(con_ptr, data) container_set(c_base(con_ptr), data, c_setup(con_ptr),  c_conflict_fix(con_ptr))

#define CN_insert(con_ptr, it, data) \
    ({ \
        It __marco_it = it; \
        container_insert(c_base(con_ptr), __marco_it, data); \
    })
// 头部插入
#define CN_add_first(con_ptr, data) CN_insert(con_ptr, CN_first(con_ptr), data)
// 尾部插入
#define CN_add_tail(con_ptr, data) CN_insert(con_ptr, CN_tail(con_ptr), data)

#define CN_add(con_ptr, data) CN_add_tail(con_ptr, data)

#define CN_remove(con_ptr, it, prdata) \
    ({ \
        It __marco_it = it; \
        int __marco_ret = -1; \
        if (It_valid(__marco_it)) { \
            container_remove(c_base(con_ptr), __marco_it, prdata); \
            __marco_ret = 0; \
        } \
        __marco_ret; \
    })
// 头部移除
#define CN_rm_first(con_ptr, prdata) CN_remove(con_ptr, CN_first(con_ptr), prdata)
// 尾部移除
#define CN_rm_last(con_ptr, prdata) CN_remove(con_ptr, CN_last(con_ptr), prdata)

#define CN_rm(con_ptr, prdata) CN_rm_last(con_ptr, prdata)
// 移除特定目标
#define CN_rm_target(con_ptr, find, pret)   \
    ({                                 \
        int ret_code = -1;             \
        It pos = CN_find(con_ptr, find);   \
        if (It_valid(pos)) {           \
            ret_code = CN_remove(con_ptr, pos, pret); \
        }                                        \
        ret_code;                                \
    })

#define CN_eliminate(con_ptr, find, match, cleanup) do { \
    It first = CN_first(con_ptr); \
    while ( !It_is_tail(first) ) { \
        if (match(It_dref(first), find) == 0) { \
            Tv __marco_rdata; \
            It __marco_rm_it = first; \
            first = It_prev(first); \
            if (CN_remove(con_ptr, __marco_rm_it, &__marco_rdata) == 0 && cleanup) { \
                cleanup(__marco_rdata); \
            } \
        } \
        first = It_next(first);\
    } \
}while(0)

//#define chas(con, find) container_has(cc(con), find, ccmp(con))
#define CN_size(con_ptr) container_size(c_base(con_ptr))
#define CN_is_empty(con_ptr) (CN_size(con_ptr) == 0)
#define CN_sort(con_ptr, sort_cmp) container_sort(c_base(con_ptr), sort_cmp)
#define CN_wring(con_ptr, cb) container_wring(c_base(con_ptr), c_search_cmp(con_ptr), cb)

#define CN_unique(con_ptr, sort_cmp, cb) do { \
    CN_sort(con_ptr, sort_cmp); \
    CN_wring(con_ptr, cb);\
}while(0)

#define CN_has(con_ptr, find)            \
    ({                               \
        It pos = CN_find(con_ptr, find); \
        int ret = It_valid(pos);     \
        ret;                         \
    })

#define CN_to_arr(con_ptr, arr) do {  \
    int i = 0;                    \
    for (It first = CN_first(con_ptr);      \
        !It_equal(first, CN_tail(con_ptr)); \
        first = It_next(first) ) {      \
            arr[i++] = It_dref(first);  \
        } \
}while(0)

#define Arr_to_cn(arr, size, transfer, con_ptr) do { \
    for (int i=0; i<size; ++i) {              \
        Tv v = transfer(arr[i]);              \
        CN_add_tail(con_ptr, v);              \
    }                                         \
}while(0)

#define CN_Access(con_ptr, step) container_access(c_base(con_ptr), step)
#define CN_fetch(con_ptr, index, rdata) \
    ({ \
        It it = CN_Access(con_ptr, index); \
        rdata = It_dref(it); \
    })
// 遍历容器，
#define CN_travel(con_ptr, handle) do {                 \
    for(It first = CN_first(con_ptr);                   \
        !It_equal(first, CN_tail(con_ptr));             \
        first = It_next(first) ) {                  \
            handle( It_dref(first) );               \
        }                                           \
}while(0)

#define CN_foreach(con_ptr, handle) CN_travel(con_ptr, handle)

#define CN_duplicate(from_ptr, to_ptr) do {\
    for(It first = CN_first(from_ptr);\
        !It_equal(first, CN_tail(from_ptr));\
        first=It_next(first)) {\
        CN_add(to_ptr, It_dref(first));\
    } \ 
} while(0)


// 两个容器合并。
#define CN_merge(con1_ptr, con2_ptr) do { \
    for(It __marco_first = CN_first(con2_ptr); !It_equal(__marco_first, CN_tail(con2_ptr)); __marco_first = It_next(__marco_first)) { \
        CN_add(con1_ptr, It_dref(__marco_first)); \
    } \
}while(0)

#define CN_initialize(con_ptr, label, search_cmp, setup, conflict_fix, ... ) do {  \
    CN_set_search_cmp(con_ptr, search_cmp);      \
    CN_set_setup(con_ptr, setup);                \
    CN_set_conflict_fix(con_ptr, conflict_fix);  \
    c_base(con_ptr) = container_create(label, __VA_ARGS__); \
}while(0)

#define CN_uninitialize(con_ptr, label, cleanup) do { \
    Cleaner cleaner = (Cleaner) cleanup;          \
    if (cleaner) {                          \
        CN_foreach(con_ptr, cleaner);            \
    }                                       \
    container_destroy(label, c_base(con_ptr));      \
    c_base(con_ptr) = NULL;                         \
}while(0)

typedef int (*Cleaner)(Tv);
typedef struct _con{
    
    container_t* _container;

    int (*_search_compare)(Tv, Tv); // 此函数用于直线的搜索
    int (*_setup)(Tv*, Tv);         // 此函数用于结构数据类型容器
    int (*_conflict_fix)(Tv*, Tv);   // 此函数用于结构数据类型容器
    int (*_cleanup_handler) (Tv);   // 此函数用户数据容器结束的清扫工作
    void (*_extra_func) (void);     // 一些其他额外需要用到的函数
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