/*
 * @Author: your name
 * @Date: 2020-12-11 13:33:48
 * @LastEditTime: 2020-12-11 13:59:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/LeList.h
 */
#ifndef _LE_LIST_H_
#define _LE_LIST_H_

#include "LeCn.h"
#include "List.h"


#define _LeList(en_search_cmp) \
    ({ \
        List ls; \
        LeCN_init(ls, list, en_search_cmp); \
        ls; \
    })

#define LeList_(ls) LeCN_uninit(ls, list)

#endif