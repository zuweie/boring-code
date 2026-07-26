/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-07-26 09:31:24
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-07-26 09:41:16
 * @FilePath: /boring-code/src/deep_learning/nnalloc/nn_alloc.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "deep_learning/compute_graph2/cg_allocator.h"

static cg_allocator_t* memory_allocator = NULL;

cg_allocator_t* Get_alloc()
{
    return memory_allocator;
}

void Set_alloc(cg_allocator_t* alloc)
{
    memory_allocator = alloc;
}