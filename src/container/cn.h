/*
 * @Author: zuweie
 * @Date: 2020-09-22 15:01:45
 * @LastEditTime: 2020-10-15 09:16:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/cn.h
 */
#ifndef _CON_H_
#define _CON_H_
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

#define CN_insert(con, it, data) container_insert(cc(con), it, data)
// 头部插入
#define CN_add_first(con, data) CN_insert(con, CN_first(con), data)
// 尾部插入
#define CN_add_tail(con, data) CN_insert(con, CN_tail(con), data)

#define CN_remove(con, it, rdata) container_remove(cc(con), it, rdata)
// 头部移除
#define CN_rm_first(con, rdata) CN_remove(con, CN_first(con), rdata)
// 尾部移除
#define CN_rm_last(con, rdata) CN_remove(con, CN_last(con), rdata)
// 移除特定目标
#define CN_rm_target(con, find, ret)   \
    ({                                 \
        int ret_code = -1;             \
        It pos = CN_find(con, find); \
        if (It_valid(pos)) {           \
            ret_code = CN_remove(con, pos, ret); \
        }                                        \
        ret_code;                                \
    })

//#define chas(con, find) container_has(cc(con), find, ccmp(con))
#define CN_size(con) container_size(cc(con))
#define CN_sort(con, cmp) container_sort(cc(con), cmp)
#define CN_has(con, find) It_valid( CN_find(con, find) )

#define CN_to_arr(con, arr) do { \
    int i = 0;                    \
    for (It first = CN_first(con);      \
        !It_equal(first, CN_tail(con)); \
        first = It_next(first) ) {      \
            arr[i++] = It_dref(first);  \
        } \
}while(0)

// 遍历容器，
#define CN_travel(con, handle) do {                 \
    for(It first = CN_first(con);                   \
        !It_equal(first, CN_tail(con));             \
        first = It_next(first) ) {                  \
            handle(first);                          \
        }                                           \
}while(0)

#define CN_initialize(con, label, cmp, ... ) do {     \
    cc(con) = container_create(label, __VA_ARGS__);   \
    if (cmp) {                                        \
        ccmp(con) = cmp;                              \
    }else{                                            \
        ccmp(con) = Tv_Equal;                         \
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

typedef int (*Cleaner)(It);
typedef struct _con{
    
    container_t* _container;
    int (*_compare)(type_value_t, type_value_t);

} Container;

static inline 
int Tv_Equal (Tv v1, Tv v2) {
    return tv_equl(v1, v2);
}

#endif