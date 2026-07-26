/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-07-26 09:31:30
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-07-26 09:35:58
 * @FilePath: /boring-code/src/deep_learning/alloc/alloc.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_ALLOC_H__
#define __NN_ALLOC_H__

typedef struct cg_allocator cg_allocator_t;

cg_allocator_t* Get_alloc();
void Set_alloc(cg_allocator_t*);

#endif