/*
 * @Author: your name
 * @Date: 2020-12-01 15:00:36
 * @LastEditTime: 2020-12-01 16:16:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/TreeComponent.h
 */
#ifndef _TREE_COMPONENT_H_
#define _TREE_COMPONENT_H_

#include "Tv.h"
#include "Entity.h"

static int
TREE_INT_INSERT_CMP (Tv v1, Tv v2){
    Entity* e1 = t2p(v1);
    Entity* e2 = t2p(v2);
    if (e1->value_index == e2->value_index) {
        for (int i=0; i<e1->value_index; ++i) {
            
            // 直接用 int 的方式来比较大小。
            int ret = Tv_cmpi(e1->tv[i], e2->tv[i]);
            if (ret !=0 )return ret;
        }
        // 相等。
        return 0;
    } else {
        return e1->value_index > e2->value_index ? 1: -1;
    }
}

#endif