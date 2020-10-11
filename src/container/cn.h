/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-20 18:51:11
 * @LastEditTime: 2020-06-13 22:40:10
 * @LastEditors: Please set LastEditors
 */
#ifndef _CON_H_
#define _CON_H_
#include "base/__container.h"
#include "it.h"
#include "tv.h"

/* container function */
#define cc(con) (((Container*)(con))->_container)
#define ccmp(con) (((Container*)(con))->_compare)
//#define setCmp(con, cmp) ((Cmp(con)=cmp)?con:con)

#define CN_first(con) container_first(cc(con))
#define CN_last(con) container_last(cc(con))
#define CN_head(con) container_head(cc(con))
#define CN_tail(con) container_tail(cc(con))

#define CN_search(con,offset,find) (ccmp(con)?container_search(cc(con), offset, find, ccmp(con)):CN_tail(con))
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
#define CN_rm_target(con, find, ret) _cn_rm_target(con, find, ret)

//#define chas(con, find) container_has(cc(con), find, ccmp(con))
#define CN_size(con) container_size(cc(con))
#define CN_sort(con, cmp) container_sort(cc(con), cmp)
#define CN_has(con, find) It_valid( CN_find(con, find) )

#define CN_to_arr(con, arr) do { \
    int i = 0;                    \
    for (it first = CN_first(con);      \
        !It_equal(first, CN_tail(con)); \
        first = It_next(first) ) {      \
            arr[i++] = It_dref(first);  \
        } \
}while(0)

#define CN_init(con, label, cmp) do {       \
    cc(con) = container_create(label);   \
    ccmp(con) = cmp;                     \
}while(0)

#define CN_free(con, label) do {         \
    container_destroy(label, cc(con)); \
    ccmp(con) = NULL;                  \
    cc(con) = NULL;                    \
}while(0)

// 遍历容器，
#define CN_travel(con, handle) do {           \
    for(it first = CN_first(con);              \
        !It_equal(first, CN_tail(con));          \
        first = It_next(first) ){handle(first);} \
}while(0)

#define CN_cleanup(con, cleanup) do {    \
    tv rdata;                            \
    while(CN_rm_last(con, &rdata) != -1){ \
        cleanup(rdata);                  \        
    }                                    \
}while(0)

typedef struct _con{
    
    container_t* _container;
    int (*_compare)(type_value_t, type_value_t);

} Container;

static inline
int _cn_rm_target (Container* con, tv target, tv* ret) 
{
    it pos = CN_find(con, target);
    if (It_valid(pos)) {
        return CN_remove(con, pos, ret);
    }else {
        return -1;
    }
}

#endif