/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-05-03 18:24:40
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-05-03 18:27:14
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_router.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_TENSOR_ROUTER_H__
#define __CG_TENSOR_ROUTER_H__

static inline int cg_tensor_coordinate_router(const int axes, const int coord[], const int stride[]) 
{
    int pos = 0;
    for (int i=0; i<axes; ++i) {
        pos += coord[i] * stride[i]
    }
    return pos;
}

#endif