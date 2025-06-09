/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-29 15:29:25
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-09 12:32:35
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/opt/cg_opt_dot.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include "../cg_debug.h"
#include "../cg_tensor.h"

static int __fp(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2, void* param)
{
    CG_DEBUG("dot operation of fp\n");
    return 0;
}

static int __bp(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2, void* param)
{
    CG_DEBUG("dot opreation of bp");
    return 0;
}
    

