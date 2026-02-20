/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-19 15:08:47
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-02-20 15:49:33
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_calflow.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cg_debug.h"
#include "cg_node.h"
#include "cg_operand.h"
#include "cg_operator.h"
#include "cg_calflow.h"

static int __do_calculate(cg_node_t* znode, int version, cg_ref_t params)
{
    cg_operand_t* operand = (cg_operand_t*) znode;

    if (operand->version < version) {
        // 此节点的数据为旧的，需要计算。
        if (cg_list_size(znode->vertex.in) == 1) {
            // 此节点需要经过 operator 节点计算
            cg_operator_t* operator = cg_list_get(znode->vertex.in, 0);
            cg_list_node_t* first   = CG_LIST_TOP(operator->_base.vertex.in);

            while (first != CG_LIST_HEAD(operator->_base.vertex.in)) {
                __do_calculate(first, version, params);
                first = first->prev;
            }
            
            operator->calculate(znode, operator->_base.vertex.in, params);
        }
        operand->version = version;
    } 
    return 0;
}

static int __do_derivative(cg_node_t* znode, int version, cg_ref_t params)
{
    cg_operand_t* operand = (cg_operand_t*) znode;
    if ()
    // 遍历
    return 0;
}

int cg_calculate(cg_node_t* znode, int version, cg_ref_t params)
{
    if (znode->node_type == e_operand && cg_list_size(znode->vertex.in) <= 1) {
        return __do_calculate(znode, version, params);
    } 
    return -1;
}

int cg_derivative(cg_node_t* znode, int version, cg_ref_t params)
{
    if (znode->node_type == e_operand && cg_list_size(znode->vertex.in) <= 1) {
         
    }
    return 0;
}

int cg_derivateive_to(cg_node_t* znode, cg_node_t* x)
{
    return 0;
}