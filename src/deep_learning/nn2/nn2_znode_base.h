/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-11-26 08:17:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-02-19 13:43:38
 * @FilePath: /boring-code/src/reinforce_learning/nn2_znode.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_2_ZNODE_BASE_H__

typedef struct matrix2 matrix2_t;
typedef struct znode2 {

    char*      name;
    matrix2_t* x;
    matrix2_t* Gx;
    struct znode2* prev;
    struct znode2* next;
    int (*calculate)(struct znode2*, matrix2_t*, void*);
    int (*gradient) (struct znode2*, matrix2_t*, void*);
    int (*recycle)  (struct znode2*);

} znode2_t;


#define __NN_2_ZNODE_BASE_H__

#endif