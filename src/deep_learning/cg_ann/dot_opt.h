/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-11 16:12:00
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-11 16:43:48
 * @FilePath: /boring-code/src/deep_learning/cg_ann/dot_opt.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DOT_OPT_H__
#define __DOT_OPT_H__
#include "deep_learning/compute_graph2/cg_opt_base.h"

typedef struct dot_opt
{
    cg_opt_base_t opt;
    
} dot_opt_t;

cg_opt_base_t* create_dot_opt(void*);

#endif