/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-07-25 23:25:10
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-09-05 14:51:49
 * @FilePath: /boring-code/src/deep_learning/nn_library/nn_operand.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_OPERAND_H__
#define __NN_OPERAND_H__
#include <stdlib.h>
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/compute_graph2/cg_operand.h"

typedef struct nn nn_t;

typedef struct nn_operand {

    cg_operand_t _base;
    cg_tensor_t* x;
    cg_tensor_t* Gx;

} nn_operand_t;

static inline int __reset(cg_operand_t* thiz) {
    cg_tensor_recycle(((nn_operand_t*)thiz)->x);
    cg_tensor_recycle(((nn_operand_t*)thiz)->Gx);
    return 0;
}

static inline nn_operand_t* nn_operand_create(cg_allocator_t* alloc, char* id, int rows, int cols)
{
    nn_operand_t* operand = (nn_operand_t*) malloc (sizeof(nn_operand_t));
    cg_operand_init(operand, id, __reset);
    operand->x  = cg_tensor_create(alloc, 2, rows, cols);
    operand->Gx = cg_tensor_create(alloc, 2, rows, cols);
    return operand;
}


#endif