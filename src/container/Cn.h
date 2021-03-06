/*
 * @Author: zuweie
 * @Date: 2020-09-22 15:01:45
 * @LastEditTime: 2020-12-10 20:02:51
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
#define cc(con)              (((Container*)(&(con)))->_container)
#define c_search_cmp(con)    (((Container*)(&(con)))->_search_compare)
#define c_conflict_fix(con)  (((Container*)(&(con)))->_conflict_fix)
#define c_setup(con)         (((Container*)(&(con)))->_setup)
#define c_extra_func(con)    (((Container*)(&(con)))->_extra_func)

#define CN_set_search_cmp(con, search_cmp)     (c_search_cmp(con)=search_cmp)
#define CN_set_setup(con, setup)               (c_setup(con) = setup)
#define CN_set_conflict_fix(con, conflict_fix) (c_conflict_fix(con)=conflict_fix)
#define CN_set_extra_func(con, func)           (c_extra_func(con)=func)

#define CN_first(con) container_first(cc(con))
#define CN_last(con) container_last(cc(con))
#define CN_head(con) container_head(cc(con))
#define CN_tail(con) container_tail(cc(con))

#define CN_search(con,offset,find) container_search(cc(con), offset, find, c_search_cmp(con))
#define CN_find(con,find) CN_search(con, CN_first(con), find)

// 特殊的插入。
#define CN_set(con, data) container_set(cc(con), data, c_setup(con),  c_conflict_fix(con))

#define CN_insert(con, it, data) \
    ({ \
        It __marco_it = it; \
        container_insert(cc(con), __marco_it, data); \
    })
// 头部插入
#define CN_add_first(con, data) CN_insert(con, CN_first(con), data)
// 尾部插入
#define CN_add_tail(con, data) CN_insert(con, CN_tail(con), data)

#define CN_add(con, data) CN_add_tail(con, data)

#define CN_remove(con, it, prdata) \
    ({ \
        It __marco_it = it; \
        int __marco_ret = -1; \
        if (It_valid(__marco_it)) { \
            container_remove(cc(con), __marco_it, prdata); \
            __marco_ret = 0; \
        } \
        __marco_ret; \
    })
// 头部移除
#define CN_rm_first(con, prdata) CN_remove(con, CN_first(con), prdata)
// 尾部移除
#define CN_rm_last(con, prdata) CN_remove(con, CN_last(con), prdata)

#define CN_rm(con, prdata) CN_rm_last(con, prdata)
// 移除特定目标
#define CN_rm_target(con, find, pret)   \
    ({                                 \
        int ret_code = -1;             \
        It pos = CN_find(con, find);   \
        if (It_valid(pos)) {           \
            ret_code = CN_remove(con, pos, pret); \
        }                                        \
        ret_code;                                \
    })

#define CN_eliminate(con, find, match, cleanup) do { \
    It first = CN_first(con); \
    while ( !It_is_tail(first) ) { \
        if (match(It_dref(first), find) == 0) { \
            Tv __marco_rdata; \
            It __marco_rm_it = first; \
            first = It_prev(first); \
            if (CN_remove(con, __marco_rm_it, &__marco_rdata) == 0 && cleanup) { \
                cleanup(__marco_rdata); \
            } \
        } \
        first = It_next(first);\
    } \
}while(0)

//#define chas(con, find) container_has(cc(con), find, ccmp(con))
#define CN_size(con) container_size(cc(con))
#define CN_is_empty(con) (CN_size(con) == 0)
#define CN_sort(con, sort_cmp) container_sort(cc(con), sort_cmp)
#define CN_wring(con, cb) container_wring(cc(con), c_search_cmp(con), cb)

#define CN_unique(con, sort_cmp, cb) do { \
    CN_sort(con, sort_cmp); \
    CN_wring(con, cb);\
}while(0)

#define CN_has(con, find)            \
    ({                               \
        It pos = CN_find(con, find); \
        int ret = It_valid(pos);     \
        ret;                         \
    })

#define CN_to_arr(con, arr) do {  \
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

#define CN_Access(con, step) container_access(cc(con), step)
#define CN_fetch(con, index, rdata) \
    ({ \
        It it = CN_Access(con, index); \
        rdata = It_dref(it); \
    })
// 遍历容器，
#define CN_travel(con, handle) do {                 \
    for(It first = CN_first(con);                   \
        !It_equal(first, CN_tail(con));             \
        first = It_next(first) ) {                  \
            handle( It_dref(first) );               \
        }                                           \
}while(0)

#define CN_foreach(con, handle) CN_travel(con, handle)

#define CN_duplicate(from, to) do {\
    for(It first = CN_first(from);\
        !It_equal(first, CN_tail(from));\
        first=It_next(first)) {\
        CN_add(to, It_dref(first));\
    }\ 
} while(0)


// 两个容器合并。
#define CN_merge(con1, con2) do { \
    for(It __marco_first = CN_first(con2); !It_equal(__marco_first, CN_tail(con2)); __marco_first = It_next(__marco_first)) { \
        CN_add(con1, It_dref(__marco_first)); \
    } \
}while(0)

#define CN_initialize(con, label, search_cmp, setup, conflict_fix, ... ) do {  \
    CN_set_search_cmp(con, search_cmp);      \
    CN_set_setup(con, setup);                \
    CN_set_conflict_fix(con, conflict_fix);  \
    cc(con) = container_create(label, __VA_ARGS__); \
}while(0)

#define CN_uninitialize(con, label, cleanup) do { \
    Cleaner cleaner = (Cleaner) cleanup;          \
    if (cleaner) {                          \
        CN_foreach(con, cleaner);            \
    }                                       \
    container_destroy(label, cc(con));      \
    cc(con) = NULL;                         \
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