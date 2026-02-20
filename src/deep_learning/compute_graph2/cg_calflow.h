/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-19 15:08:40
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-02-19 21:03:20
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_calflow.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AEER
 */
#ifndef __CG_CAL_FLOW_H__
#define __CG_CAL_FLOW_H__

typedef struct cg_node cg_node_t;

int cg_calculate(cg_node_t* znode);
int cg_derivative(cg_node_t* znode);
int cg_derivateive_to(cg_node_t* znode, cg_node_t* x);


#endif